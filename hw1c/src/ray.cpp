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
    FloatVec3 p = ray.extend(ray_t);
    const MtlColorType &cur_material = get_material(scene, obj_type, obj_idx);
    Color Od_lambda = cur_material.Od_lambda;
    // compute the coresponding color from the texture coordinate
    if (texture_map_enabled(scene, obj_type, obj_idx))
    {
        Od_lambda = get_color(scene, obj_type, obj_idx, p);
    }
    float Ir, Ig, Ib;
    float sum_r = cur_material.ka * Od_lambda.r;
    float sum_g = cur_material.ka * Od_lambda.g;
    float sum_b = cur_material.ka * Od_lambda.b;
    Color res_color;
    float f_att = 1;  // light source attenuation factor
    float depth_cue;  // depth cueing factor
    // light intensity for each component, just take the average for simplicity
    float IL = 2.0 / (scene.light_list.size() + scene.attlight_list.size());
    // illumination for normal light
    for (int i = 0; i < scene.light_list.size(); i++)
    {
        Light light = scene.light_list[i];
        res_color = light_shade(scene, ray, ray_t, light, obj_type, obj_idx);
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
        res_color = light_shade(scene, ray, ray_t, attlight, obj_type, obj_idx);
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

Color light_shade(const Scene &scene, const Ray &ray, float ray_t, const Light &light, std::string obj_type, int obj_idx)
{
    // get the intersection point
    FloatVec3 p = ray.extend(ray_t);
    const MtlColorType &cur_material = get_material(scene, obj_type, obj_idx);
    Color Od_lambda = cur_material.Od_lambda;
    Color Os_lambda = cur_material.Os_lambda;
    // compute the coresponding color from the texture coordinate
    if (texture_map_enabled(scene, obj_type, obj_idx))
    {
        Od_lambda = get_color(scene, obj_type, obj_idx, p);
    }
    FloatVec3 N = get_normal(scene, obj_type, obj_idx, p);
    // calculate vector L
    FloatVec3 L;
    if (std::abs(light.w - 1) < 1e-6) // point light source
    {
        L = FloatVec3(light.x - p.first, light.y - p.second, light.z - p.third).normal();
    }
    else if (std::abs(light.w - 0) < 1e-6) // directional light source
    {
        L = FloatVec3(-light.x, -light.y, -light.z).normal();
    }
    // calculate vector H
    // get the vector v
    FloatVec3 V = FloatVec3(-ray.dir.first, -ray.dir.second, -ray.dir.third).normal();
    FloatVec3 H;
    H = (V + L).normal();

    // apply the phong illumination model
    float Ir, Ig, Ib; // illumination components for R, G and B
    int shadow_flag = 1; // shadowing effect flag, 1 when not in shadow, 0 when in shadow

    // check for shadowing effect
    // cast a second ray forwarding from the intersection point
    // to the point light source,
    // and check for intersection with objects in the scene
    Ray ray_second =
    {
        .center = FloatVec3(p),
        .dir = FloatVec3(L)
    };
    // check intersection, if intersected, set the flag to be 0
    if (shadow_check(scene, ray_second, light, obj_idx))
    {
        shadow_flag = 0;
    }

    float term1 = std::max(float(0), N.dot(L));
    float term2 = pow(std::max(float(0), N.dot(H)), cur_material.n);
    Ir = shadow_flag * (cur_material.kd * Od_lambda.r * term1 + cur_material.ks * Os_lambda.r * term2);
    Ig = shadow_flag * (cur_material.kd * Od_lambda.g * term1 + cur_material.ks * Os_lambda.g * term2);
    Ib = shadow_flag * (cur_material.kd * Od_lambda.b * term1 + cur_material.ks * Os_lambda.b * term2);

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
    float A, B, C, D;
    float ray_t;
    float determinant;
    FloatVec3 p;  // intersection point

    // check intersection for spheres
    for (auto s:scene.sphere_list) 
    {
        B = 2 * (ray.dir.first * (ray.center.first - s.center.first) + 
                 ray.dir.second * (ray.center.second - s.center.second) + 
                 ray.dir.third * (ray.center.third - s.center.third));
        C = pow(ray.center.first - s.center.first, 2) + 
            pow(ray.center.second - s.center.second, 2) + 
            pow(ray.center.third - s.center.third, 2) - 
            pow(s.radius, 2);
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

    // check intersection for triangles
    for (auto t:scene.triangle_list)
    {
        // parameters for the plane equation Ax + By + Cz + D = 0
        FloatVec3 p0 = t.v0.p;
        FloatVec3 p1 = t.v1.p;
        FloatVec3 p2 = t.v2.p;
        FloatVec3 e1 = p1 - p0;
        FloatVec3 e2 = p2 - p0;
        FloatVec3 n = e1.cross(e2).normal();
        A = n.first;
        B = n.second;
        C = n.third;
        D = - A * p0.first - B * p0.second - C * p0.third;
        determinant = A * ray.dir.first + B * ray.dir.second + C * ray.dir.third;
        // test whether the ray is parallel with the plane
        if (std::abs(determinant) < 1e-6)
        {
            // in the case that the ray is parallel to the plane
            continue;
        }
        ray_t = -(A * ray.center.first + B * ray.center.second + C * ray.center.third + D) / determinant;
        if (ray_t < 0)
        {
            // no intersection
            continue;
        }
        // get the intersection point p
        p = ray.extend(ray_t);
        FloatVec3 bayrcentric_coordinates = t.barycentric(p);
        float alpha = bayrcentric_coordinates.first;
        float beta = bayrcentric_coordinates.second;
        float gamma = bayrcentric_coordinates.third;
        // test whether the intersection point is inside the triangle or not
        if (alpha > -1e-6 && alpha < 1 && beta > -1e-6 && beta < 1 && gamma > -1e-6 && gamma < 1)
        {
            // in the triangle
            if (ray_t < min_t && t.obj_idx != exclude_id)
            {
                min_t = ray_t;
                obj_idx = t.obj_idx;
                obj_type = "Triangle";
            }
        }
        else
        {
            // if not inside the triangle, do nothing
            continue;
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
            float max_t = (light.x - ray.center.first) / ray.dir.first;
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
    FloatVec3 raydir = (point_in_view - scene.eye).normal();
    // initialize the response color to be the background color
    Color res_color(scene.bkgcolor);

    Ray ray = 
    {
        .center = FloatVec3(scene.eye),
        .dir = FloatVec3(raydir)
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
