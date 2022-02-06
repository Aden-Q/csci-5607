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

// calculate the distance between two 2D points
float distance_between_2D_points(FloatVec2 point1, FloatVec2 point2);

// calculate the distance between two 3D points in the 3D space
float distance_between_3D_points(FloatVec3 point1, FloatVec3 point2);

// check whether a float lies within the interval determined by two end points e1 and e2
bool lie_within(float p, float e1, float e2);

// initialize a viewing window
void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist);

// ray shading
Color shade_ray(const Scene &scene, float min_t);

// trace the ray from view origin to pixel on the image and return color info
Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h);

// check ray intersection with objects in the scene and return the minimal t which leads to an intersection
int intersect_check(const Scene &scene, Ray &ray);

#endif // SRC_UTILS_H_
