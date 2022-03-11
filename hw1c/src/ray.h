/**
 * @file ray.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_RAY_H_
#define SRC_RAY_H_

// constant
#define PI 3.14159265

#include <string>
#include <tuple>
#include "types.h"

// ray shading, obj_type is the type of the object, obj_idx is used to index a particular object list
// ray_t is the parameter to define a ray
Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, Ray &ray, float ray_t);

// check whether the ray intersects with any objects in the scene
bool shadow_check(const Scene &scene, const Ray &ray, const Light &light, const int exclude_id);

// light source attenuation
float light_attenuation(const FloatVec3 &point, const AttLight &light);

// depth cueing, given the intersection point and viewer's position, return the depth cue coefficient
float depth_cueing(const FloatVec3 &point, const FloatVec3 &viewer, const DepthCueing &depth_cue);

// trace the ray from view origin to pixel on the image and return color info
Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h);

// illuminate the point using the Phong Illumination Model without attenuation or depth cueing
Color light_shade(const Scene &scene, const Ray &ray, float ray_t, const Light &light, std::string obj_type, int obj_idx);

// check ray intersection with objects in the scene and return the minimal t which leads to an intersection
// the last parameter is used to avoid self-intersection (specify a excluding object that don't need to check)
std::tuple<std::string, int, float> intersect_check(const Scene &scene, const Ray &ray, const int exclude_id);

#endif // SRC_RAY_H_
