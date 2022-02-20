/**
 * @file ray.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_RAY_H_
#define SRC_RAY_H_

// constant
#define PI 3.14159265

#include <iostream>
#include <string>
#include <tuple>
#include "types.h"

// ray shading, obj_type is the type of the object, obj_idx is used to index a particular object list
// ray_t is the parameter to define a ray
Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, Ray &ray, float ray_t);

// trace the ray from view origin to pixel on the image and return color info
Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h);

// check ray intersection with objects in the scene and return the minimal t which leads to an intersection
std::tuple<std::string, int, float> intersect_check(const Scene &scene, Ray &ray);

#endif // SRC_RAY_H_
