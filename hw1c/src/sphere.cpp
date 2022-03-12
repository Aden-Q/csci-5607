/**
 * @file sphere.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "sphere.h"

FloatVec3 Sphere::normal(const FloatVec3 &p) const
{
    return FloatVec3((p.first - this->center.first) / this->radius,
                     (p.second - this->center.second) / this->radius,
                     (p.third - this->center.third) / this->radius);
}

FloatVec2 Sphere::texture_coordinate(const FloatVec3 &p) const
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
