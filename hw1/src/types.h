/**
 * @file types.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <vector>

typedef struct
{
    float x, y, z;
    float dx, dy, dz;
} RayType;

typedef struct Sphere_Struct
{
    // material color of the sphere
    float r, g, b;
    // location and radius of the sphere
    float c_x, c_y, c_z, radius;
} Sphere;

// placeholder for cylinder

typedef struct
{
    float eye_x, eye_y, eye_z;
    float vdir_x, vdir_y, vdir_z;
    int up_x, up_y, up_z;
    int vfov;
    int width, height;
    int bkg_r, bkg_g, bkg_b;
    // A list of sphere objects
    std::vector<Sphere> sphere_list;
    // placebolder for cylinder
} Scene;

#endif // SRC_TYPES_H_
