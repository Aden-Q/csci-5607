/**
 * @file ray.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
#include "types.h"
#include "ray.h"
#include "utils.h"

Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, Ray &ray, float ray_t)
{
    // use The Phong Illumination Model to determine the color of the intersecting point
    // return the corresponding color for that object
    // the surface normal should be consider differently for sphere and cylinder
    const Sphere &sphere = scene.sphere_list[obj_idx]; 
    // compute the intersection point
    FloatVec3 p(ray.x + ray_t * ray.dx, ray.y + ray_t * ray.dy, ray.z + ray_t * ray.dz);
    // compute the surface normal N and normalize it
    FloatVec3 N((p.first - sphere.cx) / sphere.radius,
                (p.second - sphere.cy) / sphere.radius,
                (p.third - sphere.cz) / sphere.radius);
    // compute the vector L, consider differently according to the type of light source
    std::vector<FloatVec3> L_list;
    for (auto light:scene.light_list)
    {
        if (abs(light.w - 1) < 1e-6)  // point light source
        {
            FloatVec3 temp_L(light.x - p.first, light.y - p.second, light.z - p.third);
            L_list.push_back(vector_normalize(temp_L));
        }
        else if (abs(light.w - 0) < 1e-6)  // directional light source
        {
            FloatVec3 temp_L(-light.x, -light.y, -light.z);
            L_list.push_back(vector_normalize(temp_L));
        }
    }
    // compute vector H, for each L
    std::vector<FloatVec3> H_list;
    // get the vector v
    FloatVec3 temp_V(-ray.dx, -ray.dy, -ray.dz);
    FloatVec3 V = vector_normalize(temp_V);
    for (auto L:L_list)
    {
        H_list.push_back(vector_normalize(V + L));
    }

    float Ir, Ig, Ib;  // illumination components for R, G and B
    const MtlColorType &cur_material = scene.material_list[scene.sphere_list[obj_idx].m_idx];
    float sum_r = cur_material.ka * cur_material.Od_r;
    float sum_g = cur_material.ka * cur_material.Od_g;
    float sum_b = cur_material.ka * cur_material.Od_b;
    int shadow_flag = 1; // shadowing effect flag, 1 when not in shadow, 0 when in shadow

    // light intensity for each component, just take the average for simplicity
    std::vector<float> IL_list(scene.light_list.size(), 2.0 / scene.light_list.size());
    float IL;
    // light source attentuation parameters
    float att_c1 = 0.0025;
    float att_c2 = 0.0025;
    float att_c3 = 0.005;
    float f_att = 1; // attenuation factor
    float dist;  // distance
    // for each light, calculate the aculmulated color components
    for (int i = 0; i < scene.light_list.size(); i++)
    {
        shadow_flag = 1;
        f_att = 1;
        Light light = scene.light_list[i];
        // check for shadowing effect
        // cast a second ray forwarding from the intersection point
        // to the point light source,
        // and check for intersection with objects in the scene
        Ray ray_second =
            {
                .x = p.first,
                .y = p.second,
                .z = p.third,
                .dx = L_list[i].first,
                .dy = L_list[i].second,
                .dz = L_list[i].third,
            };
        // check intersection, if intersected, set the flag to be 0
        if (shadow_check(scene, ray_second, light, obj_idx))
        {
            shadow_flag = 0;
        }

        // implement light source attenuation if it is a point light source
        if (abs(light.w - 1.0) < 1e-6)
        {
            dist = distance_between_3D_points(p, FloatVec3(light.x, light.y, light.z));
            f_att = 1.0 / (att_c1 + att_c2 * dist + att_c3 * dist * dist);
            // clamping
            f_att = std::min(float(1.0), f_att);
        }

        IL = IL_list[i];
        float term1 = std::max(float(0), dot_product(N, L_list[i]));
        float term2 = pow(std::max(float(0), dot_product(N, H_list[i])), cur_material.n);
        Ir = cur_material.kd * cur_material.Od_r * term1 + cur_material.ks * cur_material.Os_r * term2;
        Ig = cur_material.kd * cur_material.Od_g * term1 + cur_material.ks * cur_material.Os_g * term2;
        Ib = cur_material.kd * cur_material.Od_b * term1 + cur_material.ks * cur_material.Os_b * term2;
        sum_r += (f_att * shadow_flag * IL * Ir);
        sum_g += (f_att * shadow_flag * IL * Ig);
        sum_b += (f_att * shadow_flag * IL * Ib);
    }

    // clamping
    sum_r = std::min(float(1.0), sum_r);
    sum_g = std::min(float(1.0), sum_g);
    sum_b = std::min(float(1.0), sum_b);

    Color res_color = {
        .r = sum_r,
        .g = sum_g,
        .b = sum_b};
    return res_color;
}

std::tuple<std::string, int, float> intersect_check(const Scene &scene, const Ray &ray, const int exclude_id)
{
    float min_t = 100000;
    float temp_t;
    int obj_idx = -1;  // the ID (index) of the intersected object
    std::string obj_type = "None";  // the type of the intersected object with the ray
    float B;
    float C;
    float determinant;

    // check intersects for spheres
    for (auto s:scene.sphere_list) 
    {
        B = 2 * (ray.dx * (ray.x - s.cx) + ray.dy * (ray.y - s.cy) + ray.dz * (ray.z - s.cz));
        C = pow(ray.x - s.cx, 2) + pow(ray.y - s.cy, 2) + pow(ray.z - s.cz, 2) - pow(s.radius, 2);
        determinant = pow(B, 2) - 4 * C;
        if (determinant > -1e-6)  // greater than or equal to 0
        {   // need further check
            temp_t = (-B - sqrt(determinant)) / 2;
            if (temp_t > 1e-6 && temp_t < min_t && s.obj_idx != exclude_id)
            {
                min_t = temp_t;
                obj_idx = s.obj_idx;
                obj_type = "Sphere";
            }
            // check for another possible solution
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 1e-6 && temp_t < min_t && s.obj_idx != exclude_id)
            {
                min_t = temp_t;
                obj_idx = s.obj_idx;
                obj_type = "Sphere";
            }
        }
    }

    return std::make_tuple(obj_type, obj_idx, min_t);
}

bool shadow_check(const Scene &scene, const Ray &ray, const Light &light, const int exclude_id)
{
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray, exclude_id);
    if (obj_type != "None")
    {
        // if it is a point light source
        // need further check whether the object is within the range between
        // the starting point of the ray and the point light source
        if ((light.w - 1) < 1e-6)
        {
            float max_t = (light.x - ray.x) / ray.dx;
            if (ray_t > 1e-6 && ray_t < max_t)
            {
                return true;
            }
        }
        else
        {
            // in the case of directional light source,
            // as long as the returned t is positive, there is shadow
            if (ray_t > 1e-6)
            {
                return true;
            }
        }
    }

    return false;
}

Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h) 
{
    // get a ray representation
    FloatVec3 point_in_view(viewwindow.ul + viewwindow.dh * w + viewwindow.dv * h);
    FloatVec3 raydir = vector_normalize(point_in_view - scene.eye);
    // initialize the response color to be the background color
    Color res_color = 
    {
        .r = scene.bkgcolor.first,
        .g = scene.bkgcolor.second,
        .b = scene.bkgcolor.third,
    };

    Ray ray = 
    {
        .x = scene.eye.first,
        .y = scene.eye.second,
        .z = scene.eye.third,
        .dx = raydir.first,
        .dy = raydir.second,
        .dz = raydir.third,
    };

    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray, -1);
    if (obj_type != "None")
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray, ray_t);
    }

    return res_color;
}
