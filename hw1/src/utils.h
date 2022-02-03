/**
 * @file utils.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

// constant
#define PI 3.14159265

#include <iostream>
#include "types.h"

// parse the scene parameters, return how many keywords are catched
int parse_scene(std::string filename, Scene &scene);

// write to the outputfile in PPM format
void output_image(std::string filename, Image **checkerboard, const Scene &scene);

// calculate the cross product of two 3d vector
FloatVec3 cross_product(const FloatVec3 &a, const FloatVec3 &b);

// normalize a 3d vector
FloatVec3 vector_normalize(const FloatVec3 &v);

// initialize a viewing window
void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist);

// ray shading
Color shade_ray(const Scene &scene, float min_t);

// trace the ray from view origin to pixel on the image and return color info
Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h);

// check ray intersection with objects in the scene and return the minimal t which leads to an intersection
int intersect_check(const Scene &scene, Ray &ray);

#endif // SRC_UTILS_H_
