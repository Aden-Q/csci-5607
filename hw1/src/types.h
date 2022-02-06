/**
 * @file types.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <vector>

typedef struct ColorType
{
    float r, g, b;
} Color;

typedef struct RayType
{
    float x, y, z;
    float dx, dy, dz;
} Ray;

typedef struct SphereType
{
    // material index
    int m_idx;
    // location and radius of the sphere
    float cx, cy, cz, radius;
} Sphere;

typedef struct CylinderType
{
    // material index
    int m_idx;
    // center location of the cylinder
    float cx, cy, cz;
    // direction of the cylinder
    float dx, dy, dz;
    // radius and length
    float radius, length;
} Cylinder;

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
    // A list of materials
    std::vector<Color> material_list;
    // A list of sphere objects
    std::vector<Sphere> sphere_list;
    // A list of cylinder
    std::vector<Cylinder> cylinder_list;
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
