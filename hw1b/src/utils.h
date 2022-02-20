/**
 * @file utils.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

// constant
#define PI 3.14159265

#include <string>
#include "types.h"

// write to the outputfile in PPM format
void output_image(std::string filename, Image **checkerboard, const Scene &scene);

// calculate the dot product of two 3d vector
float dot_product(const FloatVec3 &a, const FloatVec3 &b);

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

#endif // SRC_UTILS_H_
