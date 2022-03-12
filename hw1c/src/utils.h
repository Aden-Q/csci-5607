/**
 * @file utils.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <string>
#include <tuple>
#include "types.h"
#include "scene.h"
#include "color.h"
#include "material_color.h"
#include "texture.h"
#include "ray.h"

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
const MaterialColor &get_material(const Scene &scene, std::string obj_type, int obj_idx);

// get the unit normal vector at some point
FloatVec3 get_normal(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p);

// check whether texture map is enabled for the current object
bool texture_map_enabled(const Scene &scene, std::string obj_type, int obj_idx);

// get the intrinsic color from the texture coordinate
Color get_color(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p);

// ray shading, obj_type is the type of the object, obj_idx is used to index a particular object list
// ray_t is the parameter to define a ray
Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, Ray &ray, float ray_t);

// check whether the ray intersects with any objects in the scene
bool shadow_check(const Scene &scene, const Ray &ray, const Light &light, const int exclude_id);

// light source attenuation
float light_attenuation(const FloatVec3 &point, const AttLight &light);

// depth cueing, given the intersection point and viewer's position, return the depth cue coefficient
float depth_cueing(const FloatVec3 &point, const FloatVec3 &viewer, const DepthCue &depth_cue);

// trace the ray from view origin to pixel on the image and return color info
Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h);

// illuminate the point using the Phong Illumination Model without attenuation or depth cueing
Color light_shade(const Scene &scene, const Ray &ray, float ray_t, const Light &light, std::string obj_type, int obj_idx);

// check ray intersection with objects in the scene and return the minimal t which leads to an intersection
// the last parameter is used to avoid self-intersection (specify a excluding object that don't need to check)
std::tuple<std::string, int, float> intersect_check(const Scene &scene, const Ray &ray, const int exclude_id);

#endif // SRC_UTILS_H_
