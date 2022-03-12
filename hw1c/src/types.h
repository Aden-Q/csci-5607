/**
 * @file types.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <vector>
#include <cmath>

// const
#define MAX_VAL 255
#define PI 3.14159265

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

    // normalization
    FloatVec2 normal() const
    {
        float norm = sqrt(this->first * this->first + this->second * this->second);
        return FloatVec2(this->first / norm, this->second / norm);
    }

    // dot product
    float dot(const FloatVec2 &v) const
    {
        return (this->first * v.first + this->second * v.second);
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
    FloatVec3(float first_ = 0, float second_ = 0, float third_ = 0)
        : first(first_), second(second_), third(third_)
    {
    }

    // normalization
    FloatVec3 normal() const
    {
        float norm = sqrt(this->first * this->first + this->second * this->second + this->third * this->third);
        return FloatVec3(this->first / norm, this->second / norm, this->third / norm);
    }

    // cross product
    FloatVec3 cross(const FloatVec3 &v) const
    {
        return FloatVec3(this->second * v.third - this->third * v.second,
                         this->third * v.first - this->first * v.third,
                         this->first * v.second - this->second * v.first);
    }

    // dot product
    float dot(const FloatVec3 &v) const
    {
        return (this->first * v.first + this->second * v.second + this->third * v.third);
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

    // overload + operator, addition
    ColorType operator+(const ColorType &c) const
    {
        return ColorType(this->r + c.r, this->g + c.g, this->b + c.b);
    }

    // overload * operator, const multiple
    ColorType operator*(float c) const
    {
        return ColorType(this->r * c, this->g * c, this->b * c);
    }
} Color;

// material color type
typedef struct MtlColorType
{
    Color Od_lambda;
    Color Os_lambda;
    float ka, kd, ks;
    float n;
} MtlColor;

// texture image type
typedef struct TextureType
{
    // size of the texture image
    int width, height;
    // maximum color value
    int max_val;
    // dynamically allocate a 2d array to store pixels in the image
    Color **checkerboard;
} Texture;

typedef struct RayType
{
    // 3d origin of the ray
    FloatVec3 center;
    // direction of the ray
    FloatVec3 dir;

    // extend the ray and get a point
    FloatVec3 extend(float t) const
    {
        return FloatVec3(this->center.first + t * this->dir.first, 
                         this->center.second + t * this->dir.second, 
                         this->center.third + t * this->dir.third);
    }
} Ray;

typedef struct SphereType
{
    // object id (index into the list)
    int obj_idx;
    // material color index
    int m_idx;
    // texture index, -1 if not enable
    int texture_idx;
    // location and radius of the sphere
    FloatVec3 center;
    float radius;

    // get the unit length surface normal at a given point
    FloatVec3 normal(const FloatVec3 &p) const
    {
        return FloatVec3((p.first - this->center.first) / this->radius, 
                          (p.second - this->center.second) / this->radius, 
                          (p.third - this->center.third) / this->radius);
    }

    // compute the texture coordinate of a point on the sphere
    FloatVec2 texture_coordinate(const FloatVec3 &p) const
    {
        FloatVec3 n = this->normal(p);
        float nx = n.first;
        float ny = n.second;
        float nz = n.third;
        // add 1e-6 to avoid division by 0
        float u = 0.5 + std::atan2(ny, nx) / (2 * PI);
        float v = std::acos(nz) / PI;
        return FloatVec2(u, v);
    }
} Sphere;

typedef struct CylinderType
{
    // object id (index into the list)
    int obj_idx;
    // material color index
    int m_idx;
    // texture index, -1 if not enable
    int texture_idx;
    // center location of the cylinder
    FloatVec3 center;
    // direction of the cylinder
    FloatVec3 dir;
    // radius and length
    float radius, length;
} Cylinder;

typedef struct VertexType
{
    // object id (index into the list)
    int obj_idx;
    // position of the vertex
    FloatVec3 p;
} Vertex;

// for smooth shading
typedef struct VertexNormalType
{
    // object id (index into the list)
    int obj_idx;
    // unit normal vector
    FloatVec3 n;
} VertexNormal;

// for texture mapping
typedef struct TextureCoordinateType
{
    // object id (index into the list)
    int obj_idx;
    // texture coordinate
    FloatVec2 vt;
} TextureCoordinate;

typedef struct TriangleType
{
    // object id (index into the list)
    int obj_idx;
    // material color index
    int m_idx;
    // texture index, -1 if not enable
    int texture_idx;
    // indices of three vertex defining the triangle
    const Vertex &v0, &v1, &v2;
    // smooth shading flag, not applied by default
    bool smooth_shade;
    // indices into the array of normal directions
    int vn0_idx, vn1_idx, vn2_idx;
    // texture mapping flag, not applied by default
    bool texture_map;
    // indices into the array of texture coordinates
    int vt0_idx, vt1_idx, vt2_idx;

    // get the unit length surface normal at a given point
    FloatVec3 normal(const std::vector<VertexNormal> &vertex_normal_list, const FloatVec3 &p) const
    {
        if (!this->smooth_shade)
        {
            // in the case both smooth shading not enabled
            // return plane normal
            FloatVec3 e1 = this->v1.p - this->v0.p;
            FloatVec3 e2 = this->v2.p - this->v0.p;
            return e1.cross(e2).normal();
        }
        else
        {
            // if smooth shading enabled, return a weighted sum of 
            // vertex normals
            FloatVec3 bayrcentric_coordinates = this->barycentric(p);
            float alpha = bayrcentric_coordinates.first;
            float beta = bayrcentric_coordinates.second;
            float gamma = bayrcentric_coordinates.third;
            FloatVec3 vn0 = vertex_normal_list[this->vn0_idx - 1].n;
            FloatVec3 vn1 = vertex_normal_list[this->vn1_idx - 1].n;
            FloatVec3 vn2 = vertex_normal_list[this->vn2_idx - 1].n;
            return (vn0 * alpha + vn1 * beta + vn2 * gamma).normal();
        }
    }

    // get the barycentric coordiante of a point on the plane of the triangle
    // need to ensure that the point is on the plane
    FloatVec3 barycentric(const FloatVec3 &p) const
    {
        FloatVec3 p0 = this->v0.p;
        FloatVec3 p1 = this->v1.p;
        FloatVec3 p2 = this->v2.p;
        FloatVec3 e1 = p1 - p0;
        FloatVec3 e2 = p2 - p0;
        FloatVec3 ep = p - p0;
        float d11 = e1.dot(e1);
        float d12 = e1.dot(e2);
        float d22 = e2.dot(e2);
        float dp1 = ep.dot(e1);
        float dp2 = ep.dot(e2);
        float D = d11 * d22 - d12 * d12;
        float D_beta = d22 * dp1 - d12 * dp2;
        float D_gamma = d11 * dp2 - d12 * dp1;
        float beta = D_beta / D;
        float gamma = D_gamma / D;
        float alpha = 1 - beta - gamma;
        return FloatVec3(alpha, beta, gamma);
    }

    // compute the texture coordinate of a point on the triangle
    FloatVec2 texture_coordinate(const std::vector<TextureCoordinate> &texture_coordinate_list, const FloatVec3 &p) const
    {
        FloatVec3 barycentric_coordinate = this->barycentric(p);
        float alpha = barycentric_coordinate.first;
        float beta = barycentric_coordinate.second;
        float gamma = barycentric_coordinate.third;
        FloatVec2 vt0 = texture_coordinate_list[this->vt0_idx - 1].vt;
        FloatVec2 vt1 = texture_coordinate_list[this->vt1_idx - 1].vt;
        FloatVec2 vt2 = texture_coordinate_list[this->vt2_idx - 1].vt;
        float u = alpha * vt0.first + beta * vt1.first + gamma * vt2.first;
        float v = alpha * vt0.second + beta * vt1.second + gamma * vt2.second;
        return FloatVec2(u, v);
    }
} Triangle;

typedef struct LightType
{
    // 3d origin of the light
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
    // a list of texture images
    std::vector<Texture> texture_list;
    // a list of sphere objects
    std::vector<Sphere> sphere_list;
    // a list of cylinders
    std::vector<Cylinder> cylinder_list;
    // a list of vertexes
    std::vector<Vertex> vertex_list;
    // a list of vertex normals
    std::vector<VertexNormal> vertex_normal_list;
    // a list of texture coordinates
    std::vector<TextureCoordinate> texture_coordinate_list;
    // a list of triangles
    std::vector<Triangle> triangle_list;
    // a list of normal lights
    std::vector<Light> light_list;
    // a list of attenuated lights
    std::vector<AttLight> attlight_list;
    // depth cueing
    DepthCueing depth_cue;
    bool depth_cue_enable = false;
} Scene;

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
