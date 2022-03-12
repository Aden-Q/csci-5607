/**
 * @file cylinder.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_CYLINDER_H_
#define SRC_CYLINDER_H_

#include "types.h"

class Cylinder
{
    public:
        // constructor
        Cylinder(int obj_idx, int m_idx, int texture_idx, FloatVec3 center, FloatVec3 dir,
                 float radius, float length)
        {
            this->obj_idx = obj_idx;
            this->m_idx = m_idx;
            this->texture_idx = texture_idx;
            this->center = center;
            this->dir = dir;
            this->radius = radius;
            this->length = length;
        }

        // getter
        int getID() const { return this->obj_idx; }
        int getMidx() const { return this->m_idx; }
        int getTextureidx() const { return this->texture_idx; }
        float getRadius() const { return this->radius; }
        float getLength() const { return this->length; }
        FloatVec3 getCenter() const { return this->center; }
        FloatVec3 getDir() const { return this->dir; }

        // setter
        void setID(int obj_idx) { this->obj_idx = obj_idx; }
        void setMidx(int m_idx) { this->m_idx = m_idx; }
        void setTextureidx(int texture_idx) { this->texture_idx = texture_idx; }
        void setRadius(float radius) { this->radius = radius; }
        void setLength(float length) { this->length = length; }
        void setCenter(const FloatVec3 &center) { this->center = center; }
        void setDir(const FloatVec3 &dir) { this->dir = dir; }

    private:
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
};

#endif // SRC_CYLINDER_H_
