/**
 * @file types.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <vector>

// color type
typedef struct ColorType
{
    // r, g, b components of a color
    float r, g, b;
} Color;

// material color type
typedef struct MtlColorType
{
    float Od_r, Od_g, Od_b;
    float Os_r, Os_g, Os_b;
    float ka, kd, ks;
    float n;
} MtlColorType;

typedef struct RayType
{
    // 3d start location of the ray
    float x, y, z;
    // direction of the ray
    float dx, dy, dz;
} Ray;

typedef struct SphereType
{
    // object id (index into the list)
    int obj_idx;
    // material color index
    int m_idx;
    // location and radius of the sphere
    float cx, cy, cz, radius;
} Sphere;

typedef struct CylinderType
{
    // object id (index into the list)
    int obj_idx;
    // material color index
    int m_idx;
    // center location of the cylinder
    float cx, cy, cz;
    // direction of the cylinder
    float dx, dy, dz;
    // radius and length
    float radius, length;
} Cylinder;

typedef struct LightType
{
    // location of the light
    float x, y, z;
    // type of the light, 0 for point light source, 1 for directional light source
    float w;
    // r, g, b components of the light
    float r, g, b;
} Light;

// 2d vector
struct FloatVec2
{
    float first;
    float second;

    // default constructor
    FloatVec2(float first_ = 0, float second_ = 0)
        : first(first_), second(second_)
    {
    }

    // overload + operator
    FloatVec2 operator+(const FloatVec2 &v) const
    {
        return FloatVec2(this->first + v.first, this->second + v.second);
    }

    // overload - operator
    FloatVec2 operator-(const FloatVec2 &v) const
    {
        return FloatVec2(this->first - v.first, this->second - v.second);
    }

    // overload * operator, const multiple
    FloatVec2 operator*(float c) const
    {
        return FloatVec2(this->first * c, this->second * c);
    }

    // overload / operator, const multiple
    FloatVec2 operator/(float c) const
    {
        return FloatVec2(this->first / c, this->second / c);
    }
};

struct FloatVec3
{
    float first;
    float second;
    float third;

    // default constructor
    FloatVec3 (float first_ = 0, float second_ = 0, float third_ = 0)
        : first(first_), second(second_), third(third_)
    {
    }

    // overload + operator
    FloatVec3 operator+(const FloatVec3 &v) const
    {
        return FloatVec3(this->first + v.first, this->second + v.second, this->third + v.third);
    }

    // overload - operator
    FloatVec3 operator-(const FloatVec3 &v) const
    {
        return FloatVec3(this->first - v.first, this->second - v.second, this->third - v.third);
    }

    // overload * operator, const multiple
    FloatVec3 operator*(float c) const
    {
        return FloatVec3(this->first * c, this->second * c, this->third * c);
    }

    // overload / operator, const multiple
    FloatVec3 operator/(float c) const
    {
        return FloatVec3(this->first / c, this->second / c, this->third / c);
    }
};

typedef struct SceneType
{
    FloatVec3 eye;
    FloatVec3 viewdir;
    FloatVec3 updir;
    FloatVec3 bkgcolor;
    float vfov = 90;
    uint16_t width = 512, height = 512;
    // a list of materials
    std::vector<MtlColorType> material_list;
    // a list of sphere objects
    std::vector<Sphere> sphere_list;
    // a list of cylinders
    std::vector<Cylinder> cylinder_list;
    // a list of lights
    std::vector<Light> light_list;
} Scene;

typedef struct ImageType
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Image;

typedef struct ViewWindowType
{
    float viewdist; // view distance
    FloatVec3 u, v;
    float height, width; // size of the viewing window
    // four corner points
    FloatVec3 ul, ur, ll, lr;
    // offset vector
    FloatVec3 dh, dv;
} ViewWindow;

#endif // SRC_TYPES_H_
