/**
 * @file triangle.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "triangle.h"

FloatVec3 Triangle::barycentric(const Scene &scene, const FloatVec3 &p) const
{
    const std::vector<Vertex> &vertex_list = scene.getVertexList();
    FloatVec3 p0 = vertex_list[this->v0_idx - 1].p;
    FloatVec3 p1 = vertex_list[this->v1_idx - 1].p;
    FloatVec3 p2 = vertex_list[this->v2_idx - 1].p;
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

FloatVec3 Triangle::normal(const Scene &scene, const FloatVec3 &p) const
{
    const std::vector<Vertex> &vertex_list = scene.getVertexList();
    const std::vector<VertexNormal> &vertex_normal_list = scene.getVertexNormalList();
    if (!this->smooth_shade)
    {
        FloatVec3 p0 = vertex_list[this->v0_idx - 1].p;
        FloatVec3 p1 = vertex_list[this->v1_idx - 1].p;
        FloatVec3 p2 = vertex_list[this->v2_idx - 1].p;

        // in the case both smooth shading not enabled
        // return plane normal
        FloatVec3 e1 = p1 - p0;
        FloatVec3 e2 = p2 - p0;
        return e1.cross(e2).normal();
    }
    else
    {
        // if smooth shading enabled, return a weighted sum of
        // vertex normals
        FloatVec3 bayrcentric_coordinates = this->barycentric(scene, p);
        float alpha = bayrcentric_coordinates.first;
        float beta = bayrcentric_coordinates.second;
        float gamma = bayrcentric_coordinates.third;
        FloatVec3 vn0 = vertex_normal_list[this->vn0_idx - 1].n;
        FloatVec3 vn1 = vertex_normal_list[this->vn1_idx - 1].n;
        FloatVec3 vn2 = vertex_normal_list[this->vn2_idx - 1].n;
        return (vn0 * alpha + vn1 * beta + vn2 * gamma).normal();
    }
}

FloatVec2 Triangle::texture_coordinate(const Scene &scene, const FloatVec3 &p) const
{
    const std::vector<TextureCoordinate> &texture_coordinate_list = scene.getTextureCoordinateList();
    FloatVec3 barycentric_coordinate = this->barycentric(scene, p);
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