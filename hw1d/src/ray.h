/**
 * @file ray.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_RAY_H_
#define SRC_RAY_H_

#include "types.h"

class Ray
{
    public:
        // constructor
        Ray(FloatVec3 &center, FloatVec3 &dir)
        {
            this->center = FloatVec3(center);
            this->dir = FloatVec3(dir);
        }

        // getter
        const FloatVec3 &getCenter() const { return this->center; }
        const FloatVec3 &getDir() const { return this->dir; }

        // setter
        void setCenter(const FloatVec3 &center) { this->center = FloatVec3(center); }
        void setDir(const FloatVec3 &dir) { this->dir = FloatVec3(dir); }

        // extend the ray and get a point
        FloatVec3 extend(float t) const;

    private:
        // 3d origin of the ray
        FloatVec3 center;
        // direction of the ray
        FloatVec3 dir;
};

#endif // SRC_RAY_H_
