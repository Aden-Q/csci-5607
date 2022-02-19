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
#include "utils.h"

Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, float ray_t)
{
    // return the corresponding color for that object
    Color res = {
        .r = 1,
        .g = 1,
        .b = 1};
    // return scene.material_list[m_idx];
    return res;
}

std::tuple<std::string, int, int> intersect_check(const Scene &scene, Ray &ray)
{
    float min_t = 100000;
    float temp_t, temp_x, temp_y, temp_z;
    int res_idx = -1;  // the index of the intersected object into the list of objects
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
            if (temp_t > 0 && temp_t < min_t) 
            {
                min_t = temp_t;
                res_idx = s.m_idx;
                obj_type = "Sphere";
            }
            // check for another possible solution
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 0 && temp_t < min_t)
            {
                min_t = temp_t;
                res_idx = s.m_idx;
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
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cx - c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 0)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cx + c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 0)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
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
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cy - c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cy + c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
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
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for two base surfaces
            temp_t = (c.cz - c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                    obj_type = "Cylinder";
                }
            }
            // check for another surface
            temp_t = (c.cz + c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
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

    // now that we get both min_t (parameter for the ray) and res_idx (index for the material color)
    // we can apply the Blinn-Phong illumination model

    return std::make_tuple(obj_type, res_idx, min_t);
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
    int ray_t; // material index
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray);
    if (obj_type != "None")
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray_t);
    }

    return res_color;
}
