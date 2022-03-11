/**
 * @file utils.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "utils.h"

void output_image(std::string filename, Image **checkerboard, const Scene &scene)
{
    std::ofstream outputstream(filename, std::ios::out);

    if (!outputstream.is_open())
    {
        fprintf(stderr, "Could not open output stream with file %s\n", filename.c_str());
    }

    uint8_t pixel_counter = 0;
    // create a ppm image in P3 format
    outputstream
        << "P3" << std::endl
        << scene.width << " "
        << scene.height << std::endl
        << 255 << std::endl;
    // fill in color for each pixel
    for (uint32_t y = 0; y < scene.height; y++)
    {
        for (uint32_t x = 0; x < scene.width; x++)
        {
            outputstream << (int)checkerboard[x][y].r << " "
                         << (int)checkerboard[x][y].g << " "
                         << (int)checkerboard[x][y].b << " ";
            pixel_counter++;
            // start a new line if 4 pixels are filled
            if (pixel_counter % 4 == 0)
            {
                pixel_counter = 0;
                outputstream << std::endl;
            }
        }
    }

    outputstream.close();
    return;
}

float distance_between_2D_points(FloatVec2 point1, FloatVec2 point2)
{
    float sum = pow(point1.first - point2.first, 2) 
            + pow(point1.second - point2.second, 2);

    return sqrt(sum);
}

float distance_between_3D_points(FloatVec3 point1, FloatVec3 point2)
{
    float sum = pow(point1.first - point2.first, 2) 
            + pow(point1.second - point2.second, 2)
            + pow(point1.third - point2.third, 2);
    
    return sqrt(sum);
}

bool lie_within(float p, float e1, float e2)
{
    if (p > e1 && p < e2) 
    {
        return true;
    }
    else if (p > e2 && p < e1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist) 
{
    // directional vectors
    viewwindow.viewdist = viewdist;
    viewwindow.u = scene.viewdir.cross(scene.updir).normal();
    viewwindow.v = viewwindow.u.cross(scene.viewdir).normal();
    // size of the view window
    viewwindow.height = 2 * viewwindow.viewdist * tan(scene.vfov / 2 * PI / 180.0);
    // use aspect ratio match to determine width
    viewwindow.width = viewwindow.height * scene.width * 1.0 / scene.height;
    // calculate four corner points
    FloatVec3 n = scene.viewdir.normal();
    viewwindow.ul = scene.eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ur = scene.eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ll = scene.eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    viewwindow.lr = scene.eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    // calculate offset vector in the current viewing window (per pixel offset)
    viewwindow.dh = (viewwindow.ur - viewwindow.ul) / (scene.width - 1);
    viewwindow.dv = (viewwindow.ll - viewwindow.ul) / (scene.height - 1);   
}

const MtlColorType &get_material(const Scene &scene, std::string obj_type, int obj_idx)
{
    if (obj_type == "Sphere")
    {
        return scene.material_list[scene.sphere_list[obj_idx].m_idx];
    }
    else if (obj_type == "Triangle")
    {
        return scene.material_list[scene.triangle_list[obj_idx].m_idx];
    }
    else
    {
        return scene.material_list[scene.triangle_list[obj_idx].m_idx];
    }
}

FloatVec3 get_normal(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p)
{
    if (obj_type == "Sphere")
    {
        return scene.sphere_list[obj_idx].normal(p);
    }
    else if (obj_type == "Triangle")
    {
        return scene.triangle_list[obj_idx].normal(scene.vertex_normal_list ,p);
    }
    else
    {
        return scene.triangle_list[obj_idx].normal(scene.vertex_normal_list, p);
    }
}