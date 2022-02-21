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
    // default constructor
    ColorType(float r_ = 0, float g_ = 0, float b_ = 0)
        : r(r_), g(g_), b(b_)
    {
    }
} Color;

// material color type
typedef struct MtlColorType
{
    float Od_r, Od_g, Od_b;
    float Os_r, Os_g, Os_b;
    float ka, kd, ks;
    float n;
} MtlColor;

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
    // default constructor
    LightType(float x_ = 0, float y_ = 0, float z_ = 0,
              float w_ = 0, float r_ = 0, float g_ = 0,
              float b_ = 0)
        : x(x_), y(y_), z(z_), w(w_), r(r_), g(g_), b(b_)
    {
    }
} Light;

// inherite from the light class 
typedef struct AttLightType : Light
{
    // attenuation parameters
    float c1, c2, c3;
    // default constructor
    AttLightType(float x_ = 0, float y_ = 0, float z_ = 0,
                 float w_ = 0, float r_ = 0, float g_ = 0,
                 float b_ = 0, float c1_ = 0, float c2_ = 0,
                 float c3_ = 0)
        : Light(x_, y_, z_, w_, r_, g_, b_), c1(c1_), c2(c2_), c3(c3_)
    {
    }
} AttLight;

typedef struct DepthCueingType
{
    // depth cue color
    float dc_r, dc_g, dc_b;
    // upper and lower bound of the depth cue parameter
    float alpha_max, alpha_min;
    // upper and lower bound if distance
    float dist_max, dist_min;
} DepthCueing;

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
    Color bkgcolor;
    float vfov = 90;
    uint16_t width = 512, height = 512;
    // a list of materials
    std::vector<MtlColorType> material_list;
    // a list of sphere objects
    std::vector<Sphere> sphere_list;
    // a list of cylinders
    std::vector<Cylinder> cylinder_list;
    // a list of normal lights
    std::vector<Light> light_list;
    // a list of attenuated lights
    std::vector<AttLight> attlight_list;
    // depth cueing
    DepthCueing depth_cue;
    bool depth_cue_enable = false;
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
