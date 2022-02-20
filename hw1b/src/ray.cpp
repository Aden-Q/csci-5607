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
    // for each light, calculate the aculmulated color components
    for (int i = 0; i < scene.light_list.size(); i++)
    {
        shadow_flag = 1;
        Light light = scene.light_list[i];
        // check for shadowing effect when it is a point light source
        if (abs(light.w - 1) < 1e-6)
        {   
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
            if (shadow_check(scene, ray_second, light))
            {
                shadow_flag = 0;
            }
        }

        IL = IL_list[i];
        float term1 = std::max(float(0), dot_product(N, L_list[i]));
        float term2 = pow(std::max(float(0), dot_product(N, H_list[i])), cur_material.n);
        Ir = cur_material.kd * cur_material.Od_r * term1 + cur_material.ks * cur_material.Os_r * term2;
        Ig = cur_material.kd * cur_material.Od_g * term1 + cur_material.ks * cur_material.Os_g * term2;
        Ib = cur_material.kd * cur_material.Od_b * term1 + cur_material.ks * cur_material.Os_b * term2;
        sum_r += (shadow_flag * IL * Ir);
        sum_g += (shadow_flag * IL * Ig);
        sum_b += (shadow_flag * IL * Ib);
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

std::tuple<std::string, int, float> intersect_check(const Scene &scene, const Ray &ray)
{
    float min_t = 100000;
    float temp_t, temp_x, temp_y, temp_z;
    int obj_idx = -1;  // the ID (index) of the intersected object
    std::string obj_type = "None";  // the type of the intersected object with the ray
    float A;
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
            if (temp_t > 1e-2 && temp_t < min_t) 
            {
                min_t = temp_t;
                obj_idx = s.obj_idx;
                obj_type = "Sphere";
            }
            // check for another possible solution
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 1e-2 && temp_t < min_t)
            {
                min_t = temp_t;
                obj_idx = s.obj_idx;
                obj_type = "Sphere";
            }
        }
    }

    // check intersects for cylinders
    for (auto c:scene.cylinder_list) 
    {
        // std::cout << ray.x << " " << ray.y << std::endl;
        // consider three cases: three possible directions for the cylinder
        if (c.dx > 1e-6) // the cylinder heads towards positive x axis
        {
            // check for side surface
            A = 1 - pow(ray.dx, 2);  // A is not equal to 0 in this case
            B = 2 * (ray.dy * (ray.y - c.cy) + ray.dz * (ray.z - c.cz));
            C = pow(ray.y - c.cy, 2) + pow(ray.z - c.cz, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {                        // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cx - c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 1e-2)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cx + c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 1e-2)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
        }
        else if (c.dy > 1e-6) // the cylinder heads towards positive y axis
        {
            A = 1 - pow(ray.dy, 2); // A is not equal to 0 in this case
            B = 2 * (ray.dx * (ray.x - c.cx) + ray.dz * (ray.z - c.cz));
            C = pow(ray.x - c.cx, 2) + pow(ray.z - c.cz, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {   // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cy - c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 1e-2)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cy + c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 1e-2)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
        }
        else if (c.dz > 1e-6) // the cylinder heads towards positive z axis
        {
            A = 1 - pow(ray.dz, 2); // A is not equal to 0 in this case
            B = 2 * (ray.dx * (ray.x - c.cx) + ray.dy * (ray.y - c.cy));
            C = pow(ray.x - c.cx, 2) + pow(ray.y - c.cy, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {                    // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 1e-2 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cz - c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 1e-2)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cz + c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 1e-2)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    obj_idx = c.obj_idx;
                    obj_type = "Cylinder";
                }
            }
        }
        else
        {
            // placeholder here
            // don't need to consider other possible directions for assignment 1a
        }
    }

    return std::make_tuple(obj_type, obj_idx, min_t);
}

bool shadow_check(const Scene &scene, const Ray &ray, const Light &light)
{
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray);
    if (obj_type != "None")
    {
        // need further check whether the object is within the range between
        // the starting point of the ray and the point light source
        float max_t = (light.x - ray.x) / ray.dx;
        if (ray_t > 1e-6 && ray_t < max_t)
        {
            return true;
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
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray);
    if (obj_type != "None")
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray, ray_t);
    }

    return res_color;
}
