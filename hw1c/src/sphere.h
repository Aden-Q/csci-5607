/**
 * @file sphere.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_SPHERE_H_
#define SRC_SPHERE_H_

#include "types.h"

class Sphere
{
    public:
        // constructor
        Sphere(int obj_idx, int m_idx, int texture_idx, int bump_idx, FloatVec3 center, float radius)
        {
            this->obj_idx = obj_idx;
            this->m_idx = m_idx;
            this->texture_idx = texture_idx;
            this->bump_idx = bump_idx;
            this->center = center;
            this->radius = radius;
        }

        // getter
        int getID() const { return this->obj_idx; }
        int getMidx() const { return this->m_idx; }
        int getTextureidx() const { return this->texture_idx; }
        int getBumpidx() const { return this->bump_idx; }
        float getRadius() const { return this->radius; }
        FloatVec3 getCenter() const { return this->center; }

        // setter
        void setID(int obj_idx) { this->obj_idx = obj_idx; }
        void setMidx(int m_idx) { this->m_idx = m_idx; }
        void setTextureidx(int texture_idx) { this->texture_idx = texture_idx; }
        void setBumpidx(int bump_idx) { this->bump_idx = bump_idx; }
        void setRadius(float radius) { this->radius = radius; }
        void setCenter(const FloatVec3& center) { this->center = center; }

        // get the unit length surface normal at a given point
        FloatVec3 normal(const FloatVec3 &p) const;
        // compute the texture coordinate of a point on the sphere
        FloatVec2 texture_coordinate(const FloatVec3 &p) const;

    private:
        // object id (index into the list)
        int obj_idx;
        // material color index
        int m_idx;
        // texture index, -1 if not enable
        int texture_idx;
        // normal map index, -1 if not enable
        int bump_idx;
        // location and radius of the sphere
        FloatVec3 center;
        float radius;
};

#endif // SRC_SPHERE_H_
