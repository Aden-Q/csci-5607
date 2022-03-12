/**
 * @file triangle.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_

#include "types.h"
#include "scene.h"

class Scene;

class Triangle
{
    public:
        // constructor
        Triangle(int obj_idx = -1, int m_idx = -1, int texture_idx = -1, int bump_idx = -1,
                 int v0_idx = -1, int v1_idx = -1, int v2_idx = -1,
                 bool smooth_shade = false,
                 int vn0_idx = -1, int vn1_idx = -1, int vn2_idx = -1,
                 bool texture_map = false,
                 int vt0_idx = -1, int vt1_idx = -1, int vt2_idx = -1)
        {
            this->obj_idx = obj_idx;
            this->m_idx = m_idx;
            this->texture_idx = texture_idx;
            this->bump_idx = bump_idx;
            // indices into three vertexes
            this->v0_idx = v0_idx;
            this->v1_idx = v1_idx;
            this->v2_idx = v2_idx;
            this->smooth_shade = smooth_shade;
            this->vn0_idx = vn0_idx;
            this->vn1_idx = vn1_idx;
            this->vn2_idx = vn2_idx;
            this->texture_map = texture_map;
            this->vt0_idx = vt0_idx;
            this->vt1_idx = vt1_idx;
            this->vt2_idx = vt2_idx;
        }

        // getter
        int getID() const { return this->obj_idx; }
        int getMidx() const { return this->m_idx; }
        int getTextureidx() const { return this->texture_idx; }
        int getBumpidx() const { return this->bump_idx; }
        int getV0idx() const { return this->v0_idx; }
        int getV1idx() const { return this->v1_idx; }
        int getV2idx() const { return this->v2_idx; }
        bool getSmoothShade() const { return this->smooth_shade; }
        int getVn0idx() const { return this->vn0_idx; }
        int getVn1idx() const { return this->vn1_idx; }
        int getVn2idx() const { return this->vn2_idx; }
        bool getTextureMap() const { return this->texture_map; }
        int getVt0idx() const { return this->vt0_idx; }
        int getVt1idx() const { return this->vt1_idx; }
        int getVt2idx() const { return this->vt2_idx; }

        // setter
        void setID(int obj_idx) { this->obj_idx = obj_idx; }
        void setMidx(int m_idx) { this->m_idx = m_idx; }
        void setTextureidx(int texture_idx) { this->texture_idx = texture_idx; }
        void setBumpidx(int bump_idx) { this->bump_idx = bump_idx; }
        void setV0idx(int v0_idx){ this->v0_idx = v0_idx; }
        void setV1idx(int v1_idx) { this->v1_idx = v1_idx; }
        void setV2idx(int v2_idx) { this->v2_idx = v2_idx; }
        void setSmoothShade(bool smooth_shade) { this->smooth_shade = smooth_shade; }
        void setVn0idx(int vn0_idx) { this->vn0_idx = vn0_idx; }
        void setVn1idx(int vn1_idx) { this->vn1_idx = vn1_idx; }
        void setVn2idx(int vn2_idx) { this->vn2_idx = vn2_idx; }
        void setTextureMap(bool texture_map) { this->texture_map = texture_map; }
        void setVt0idx(int vt0_idx) { this->vt0_idx = vt0_idx; }
        void setVt1idx(int vt1_idx) { this->vt1_idx = vt1_idx; }
        void setVt2idx(int vt2_idx) { this->vt2_idx = vt2_idx; }

        // get the barycentric coordiante of a point on the plane of the triangle
        // need to ensure that the point is on the plane
        FloatVec3 barycentric(const Scene &scene, const FloatVec3 &p) const;
        // get the unit length surface normal at a given point
        FloatVec3 normal(const Scene &scene, const FloatVec3 &p) const;
        // compute the texture coordinate of a point on the sphere
        FloatVec2 texture_coordinate(const Scene &scene, const FloatVec3 &p) const;

    private:
        // object id (index into the list)
        int obj_idx;
        // material color index
        int m_idx;
        // texture index, -1 if not enable
        int texture_idx;
        // normal map index, -1 if not enable
        int bump_idx;
        // indices of three vertex defining the triangle
        int v0_idx, v1_idx, v2_idx;
        // smooth shading flag, not applied by default
        bool smooth_shade;
        // indices into the array of normal directions
        int vn0_idx, vn1_idx, vn2_idx;
        // texture mapping flag, not applied by default
        bool texture_map;
        // indices into the array of texture coordinates
        int vt0_idx, vt1_idx, vt2_idx;
};

#endif // SRC_TRIANGLE_H_
