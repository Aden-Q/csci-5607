/**
 * @file utils.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
#include "types.h"

// write to the outputfile in PPM format
void output_image(std::string filename, Color **checkerboard, int width, int height);

// calculate the distance between two 2D points
float distance_between_2D_points(FloatVec2 point1, FloatVec2 point2);

// calculate the distance between two 3D points in the 3D space
float distance_between_3D_points(FloatVec3 point1, FloatVec3 point2);

// check whether a float lies within the interval determined by two end points e1 and e2
bool lie_within(float p, float e1, float e2);

// initialize a viewing window
void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist);

// get the material for illumination
const MtlColorType &get_material(const Scene &scene, std::string obj_type, int obj_idx);

// get the unit normal vector at some point
FloatVec3 get_normal(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p);

// check whether texture map is enabled for the current object
bool texture_map_enabled(const Scene &scene, std::string obj_type, int obj_idx);

// get the intrinsic color from the texture coordinate
Color get_color(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p);

#endif // SRC_UTILS_H_
