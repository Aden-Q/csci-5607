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
    // compute the intersection point
    FloatVec3 p(ray.x + ray_t * ray.dx, ray.y + ray_t * ray.dy, ray.z + ray_t * ray.dz);
    const MtlColorType &cur_material = scene.material_list[scene.sphere_list[obj_idx].m_idx];
    float Ir, Ig, Ib;
    float sum_r = cur_material.ka * cur_material.Od_r;
    float sum_g = cur_material.ka * cur_material.Od_g;
    float sum_b = cur_material.ka * cur_material.Od_b;
    Color res_color;
    float f_att = 1;  // light source attenuation factor
    float depth_cue;  // depth cueing factor
    // light intensity for each component, just take the average for simplicity
    float IL = 2.0 / (scene.light_list.size() + scene.attlight_list.size());
    // illumination for normal light
    for (int i = 0; i < scene.light_list.size(); i++)
    {
        Light light = scene.light_list[i];
        res_color = light_shade(scene, ray, ray_t, light, obj_idx);
        Ir = res_color.r;
        Ig = res_color.g;
        Ib = res_color.b;

        sum_r += IL * Ir;
        sum_g += IL * Ig;
        sum_b += IL * Ib;
    }
    // illumination for attenuated light
    for (int i = 0; i < scene.attlight_list.size(); i++)
    {
        AttLight attlight = scene.attlight_list[i];
        res_color = light_shade(scene, ray, ray_t, attlight, obj_idx);
        Ir = res_color.r;
        Ig = res_color.g;
        Ib = res_color.b;
        if ((attlight.w - 1) < 1e-6)
        {
            f_att = light_attenuation(p, attlight);
        }

        sum_r += f_att * IL * Ir;
        sum_g += f_att * IL * Ig;
        sum_b += f_att * IL * Ib;
    }

    // clamping
    sum_r = std::min(float(1.0), sum_r);
    sum_g = std::min(float(1.0), sum_g);
    sum_b = std::min(float(1.0), sum_b);

    // apply depth cueing if enabled
    if (scene.depth_cue_enable)
    {
        depth_cue = depth_cueing(p, scene.eye, scene.depth_cue);
        sum_r = depth_cue * sum_r + (1 - depth_cue) * scene.depth_cue.dc_r;
        sum_g = depth_cue * sum_g + (1 - depth_cue) * scene.depth_cue.dc_g;
        sum_b = depth_cue * sum_b + (1 - depth_cue) * scene.depth_cue.dc_b;
    }

    return Color(sum_r, sum_g, sum_b);
}

Color light_shade(const Scene &scene, const Ray &ray, float ray_t, const Light &light, int obj_idx)
{
    const Sphere &sphere = scene.sphere_list[obj_idx];
    // compute the intersection point
    FloatVec3 p(ray.x + ray_t * ray.dx, ray.y + ray_t * ray.dy, ray.z + ray_t * ray.dz);
    // compute the surface normal N and normalize it
    FloatVec3 N((p.first - sphere.cx) / sphere.radius,
                (p.second - sphere.cy) / sphere.radius,
                (p.third - sphere.cz) / sphere.radius);

    // calculate vector L
    FloatVec3 temp, L;
    if (std::abs(light.w - 1) < 1e-6) // point light source
    {
        FloatVec3 temp(light.x - p.first, light.y - p.second, light.z - p.third);
        L = vector_normalize(temp);
    }
    else if (std::abs(light.w - 0) < 1e-6) // directional light source
    {
        FloatVec3 temp(-light.x, -light.y, -light.z);
        L = vector_normalize(temp);
    }
    // calculate vector H
    // get the vector v
    FloatVec3 temp_V(-ray.dx, -ray.dy, -ray.dz);
    FloatVec3 V = vector_normalize(temp_V);
    FloatVec3 H;
    H = vector_normalize(V + L);

    // apply the phong illumination model
    float Ir, Ig, Ib; // illumination components for R, G and B
    const MtlColorType &cur_material = scene.material_list[scene.sphere_list[obj_idx].m_idx];
    int shadow_flag = 1; // shadowing effect flag, 1 when not in shadow, 0 when in shadow

    // check for shadowing effect
    // cast a second ray forwarding from the intersection point
    // to the point light source,
    // and check for intersection with objects in the scene
    Ray ray_second =
        {
            .x = p.first,
            .y = p.second,
            .z = p.third,
            .dx = L.first,
            .dy = L.second,
            .dz = L.third,
        };
    // check intersection, if intersected, set the flag to be 0
    if (shadow_check(scene, ray_second, light, obj_idx))
    {
        shadow_flag = 0;
    }

    float term1 = std::max(float(0), dot_product(N, L));
    float term2 = pow(std::max(float(0), dot_product(N, H)), cur_material.n);
    Ir = (cur_material.kd * cur_material.Od_r * term1 + cur_material.ks * cur_material.Os_r * term2);
    Ig = (cur_material.kd * cur_material.Od_g * term1 + cur_material.ks * cur_material.Os_g * term2);
    Ib = (cur_material.kd * cur_material.Od_b * term1 + cur_material.ks * cur_material.Os_b * term2);

    return Color(Ir, Ig, Ib);
}

float light_attenuation(const FloatVec3 &point, const AttLight &light)
{
    // light source attentuation parameters
    float c1 = light.c1;
    float c2 = light.c2;
    float c3 = light.c3;
    float f_att = 1; // attenuation factor
    float dist;      // distance

    if (std::abs(light.w - 1.0) < 1e-6)
    {
        dist = distance_between_3D_points(point, FloatVec3(light.x, light.y, light.z));
        f_att = 1.0 / (c1 + c2 * dist + c3 * dist * dist);
        // clamping
        f_att = std::min(float(1.0), f_att);
    }
    else
    {
        f_att = 1;
    }

    return f_att;
}

float depth_cueing(const FloatVec3 &point, const FloatVec3 &viewer, const DepthCueing &depth_cue)
{
    float alpha_max = depth_cue.alpha_max;
    float alpha_min = depth_cue.alpha_min;
    float d_near = depth_cue.dist_min;
    float d_far = depth_cue.dist_max;
    float alpha = 1.0;
    float d_obj = distance_between_3D_points(point, viewer);
    // consider three difference cases: whether the object is near, far from the viewer
    // apply different formula according to the distance between the two
    if (d_obj < d_near)
    {
        alpha = alpha_max;
    }
    else if (d_obj < d_far)
    {
        alpha = alpha_min + (alpha_max - alpha_min) * (d_far - d_obj) / (d_far - d_near);
    }
    else
    {
        alpha = alpha_min;
    }

    return alpha;
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
    Color res_color(scene.bkgcolor);

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
