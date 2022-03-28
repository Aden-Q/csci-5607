/**
 * @file scene.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "types.h"
#include "color.h"
#include "material_color.h"
#include "texture.h"
#include "bump.h"
#include "sphere.h"
#include "cylinder.h"
#include "triangle.h"

class Triangle;

class Scene
{
    public:
        // default constructor
        Scene();

        // getters
        const FloatVec3 &getEye() const { return this->eye; }
        const FloatVec3 &getViewdir() const { return this->viewdir; }
        const FloatVec3 &getUpdir() const { return this->updir; }
        const Color &getBkgcolor() const { return this->bkgcolor; }
        float getVfov() const { return this->vfov; }
        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }
        const std::vector<MaterialColor> &getMaterialList() const { return this->material_list; }
        const std::vector<Texture> &getTextureList() const { return this->texture_list; }
        const std::vector<Bump> &getBumpList() const { return this->bump_list; }
        const std::vector<Sphere> &getSphereList() const { return this->sphere_list; }
        const std::vector<Cylinder> &getCylinderList() const { return this->cylinder_list; }
        const std::vector<Vertex> &getVertexList() const { return this->vertex_list; }
        const std::vector<VertexNormal> &getVertexNormalList() const { return this->vertex_normal_list; }
        const std::vector<TextureCoordinate> &getTextureCoordinateList() const { return this->texture_coordinate_list; }
        const std::vector<Triangle> &getTriangleList() const { return this->triangle_list; }
        const std::vector<Light> &getLightList() const { return this->light_list; }
        const std::vector<AttLight> &getAttLightList() const { return this->attlight_list; }
        const DepthCue &getDepthCue() const { return this->depth_cue; }
        bool depthCueEnable() const { return this->depth_cue_enable; }

        // setters
        void setEye(const FloatVec3 &eye) { this->eye = FloatVec3(eye); }
        void setViewdir(const FloatVec3 &viewdir) { this->viewdir = FloatVec3(viewdir); }
        void setUpdir(const FloatVec3 &updir) { this->updir = FloatVec3(updir); }
        void setBkgcolor(const Color &bkgcolor) { this->bkgcolor = Color(bkgcolor); }
        void setVfov(float vfov) { this->vfov = vfov; }
        void setWidth(int width) { this->width = width; }
        void setHeight(int height) { this->height = height; }
        void setMaterialList(const std::vector<MaterialColor> &material_list) { this->material_list = std::vector<MaterialColor>(material_list); }
        void setTextureList(const std::vector<Texture> &texture_list) { this->texture_list = std::vector<Texture>(texture_list); }
        void setBumpList(const std::vector<Bump> &bump_list) { this->bump_list = std::vector<Bump>(bump_list); }
        void setSphereList(const std::vector<Sphere> &sphere_list) { this->sphere_list = std::vector<Sphere>(sphere_list); }
        void setCylinderList(const std::vector<Cylinder> &cylinder_list) { this->cylinder_list = std::vector<Cylinder>(cylinder_list); }
        void setVertexList(const std::vector<Vertex> &vertex_list) { this->vertex_list = std::vector<Vertex>(vertex_list); }
        void setVertexNormalList(const std::vector<VertexNormal> &vertex_normal_list) { this->vertex_normal_list = std::vector<VertexNormal>(vertex_normal_list); }
        void setTextureCoordinateList(const std::vector<TextureCoordinate> &texture_coordinate_list) { this->texture_coordinate_list = std::vector<TextureCoordinate>(texture_coordinate_list); }
        void setTriangleList(const std::vector<Triangle> &triangle_list) { this->triangle_list = std::vector<Triangle>(triangle_list); }
        void setLightList(const std::vector<Light> &light_list) { this->light_list = std::vector<Light>(light_list); }
        void setAttLightList(const std::vector<AttLight> &attlight_list) { this->attlight_list = std::vector<AttLight>(attlight_list); }
        void setDepthCue(const DepthCue &depth_cue){ this->depth_cue = depth_cue; }

        // parse the scene parameters from the input file, return the number of keywords catched
        int parseScene(std::string filename);

    private:
        FloatVec3 eye;
        FloatVec3 viewdir;
        FloatVec3 updir;
        Color bkgcolor;
        float vfov;
        int width, height;
        // a list of materials
        std::vector<MaterialColor> material_list;
        // a list of texture images
        std::vector<Texture> texture_list;
        // a list of normal images
        std::vector<Bump> bump_list;
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
        DepthCue depth_cue;
        bool depth_cue_enable;
};

#endif // SRC_SCENE_H_
