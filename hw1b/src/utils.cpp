/**
 * @file utils.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
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

float dot_product(const FloatVec3 &a, const FloatVec3 &b)
{
    return a.first * b.first + a.second * b.second + a.third * b.third;
}

FloatVec3 cross_product(const FloatVec3 &a, const FloatVec3 &b) 
{
    return FloatVec3(a.second * b.third - a.third * b.second,
                     a.third * b.first - a.first * b.third,
                     a.first * b.second - a.second * b.first);
}

FloatVec3 vector_normalize(const FloatVec3 &v) 
{
    float n = sqrt(pow(v.first, 2) + pow(v.second, 2) + pow(v.third, 2));

    return FloatVec3(v.first / n, v.second / n, v.third / n);
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
    viewwindow.u = vector_normalize(cross_product(scene.viewdir, scene.updir));
    viewwindow.v = vector_normalize(cross_product(viewwindow.u, scene.viewdir));
    // size of the view window
    viewwindow.height = 2 * viewwindow.viewdist * tan(scene.vfov / 2 * PI / 180.0);
    // use aspect ratio match to determine width
    viewwindow.width = viewwindow.height * scene.width * 1.0 / scene.height;
    // calculate four corner points
    FloatVec3 n = vector_normalize(scene.viewdir);
    viewwindow.ul = scene.eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ur = scene.eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ll = scene.eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    viewwindow.lr = scene.eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    // calculate offset vector in the current viewing window (per pixel offset)
    viewwindow.dh = (viewwindow.ur - viewwindow.ul) / (scene.width - 1);
    viewwindow.dv = (viewwindow.ll - viewwindow.ul) / (scene.height - 1);   
}
