/**
 * @file utils.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "types.h"
#include "utils.h"

void parse_scene(std::string filename, Scene &scene)
{
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);

    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string keyword;
    std::string a, b, c, d;
    int m_idx = -1;

    while (inputstream >> keyword)
    {
        // check keywords
        if (keyword == "eye")
        {
            // read the world coordinate of the eye
            inputstream >> a >> b >> c;
            FloatVec3 eye(std::stof(a), std::stof(b), std::stof(c));
            scene.eye = eye;
        }
        else if (keyword == "viewdir")
        {
            // read the world coordinate of the view direction
            inputstream >> a >> b >> c;
            FloatVec3 viewdir(std::stof(a), std::stof(b), std::stof(c));
            scene.viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            // read the world coordinate of the up direction
            inputstream >> a >> b >> c;
            FloatVec3 updir(std::stof(a), std::stof(b), std::stof(c));
            scene.updir = updir;
        }
        else if (keyword == "vfov")
        {
            // read the vertical field of view (in degree)
            inputstream >> a;
            scene.vfov = std::stof(a);
        }
        else if (keyword == "imsize")
        {
            // read width and height of the image
            inputstream >> a >> b;
            scene.width = std::stoi(a);
            scene.height = std::stoi(b);
        }
        else if (keyword == "bkgcolor")
        {
            // read the background color
            inputstream >> a >> b >> c;
            FloatVec3 bkgcolor(std::stof(a), std::stof(b), std::stof(c));
            scene.bkgcolor = bkgcolor;
        }
        else if (keyword == "mtlcolor")
        {
            // update the current material color
            m_idx++;
            inputstream >> a >> b >> c;
            Color material = {
                .r = std::stof(a),
                .g = std::stof(b),
                .b = std::stof(c),
            };
            scene.material_list.push_back(material);
        }
        else if (keyword == "sphere")
        {
            // rendering the sphere with the current material color
            // store the spacial location and radius of the sphere
            inputstream >> a >> b >> c >> d;
            Sphere sphere = {
                .m_idx = m_idx,
                .c_x = std::stof(a),
                .c_y = std::stof(b),
                .c_z = std::stof(c),
                .radius = std::stof(d)};
            scene.sphere_list.push_back(sphere);
        }
    }

    inputstream.close();
    return;
}

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
    for (uint32_t y = 0; y < scene.width; y++)
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

FloatVec3 cross_product(const FloatVec3 &a, const FloatVec3 &b) {
    return FloatVec3(a.second * b.third - a.third * b.second,
                     a.third * b.first - a.first * b.third,
                     a.first * b.second - a.second * b.first);
}

FloatVec3 vector_normalize(const FloatVec3 &v) {
    float n = sqrt(pow(v.first, 2) + pow(v.second, 2) + pow(v.third, 2));

    return FloatVec3(v.first / n, v.second / n, v.third / n);
}

void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist) {
    // directional vectors
    viewwindow.viewdist = viewdist;
    viewwindow.u = vector_normalize(cross_product(scene.viewdir, scene.updir));
    viewwindow.v = vector_normalize(cross_product(viewwindow.u, scene.viewdir));
    // size of the view window
    viewwindow.height = 2 * viewwindow.viewdist * tan(scene.vfov / 2 * PI / 180.0);
    // use aspect ratio match to determine width
    viewwindow.width = viewwindow.height * scene.width / scene.height;
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

Color shade_ray(const Scene &scene, float m_idx) {
    // return the corresponding color for that object
    return scene.material_list[m_idx];
}

int intersect_check(const Scene &scene, Ray &ray)
{
    float min_t = 100000;
    float temp_t;
    int res_idx = -1;
    float B;
    float C;
    float determinant;
    // check for spheres
    for (auto s:scene.sphere_list) {
        B = 2 * (ray.dx * (ray.x - s.c_x) + ray.dy * (ray.y - s.c_y) + ray.dz * (ray.z - s.c_z));
        C = pow(ray.x - s.c_x, 2) + pow(ray.y - s.c_y, 2) + pow(ray.z - s.c_z, 2) - pow(s.radius, 2);
        determinant = pow(B, 2) - 4 *C;
        if (determinant > 0)  // greater than 0
        { // need further check
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 0 && temp_t < min_t) {
                min_t = temp_t;
                res_idx = s.m_idx;
            }
        }
    }

    return res_idx;
}

Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h) {
    // get a ray representation
    FloatVec3 point_in_view(viewwindow.ul + viewwindow.dh * w + viewwindow.dv * h);
    FloatVec3 raydir = vector_normalize(point_in_view - scene.eye);
    // initialize the response color to be the background color
    Color res_color = {
        .r = scene.bkgcolor.first,
        .g = scene.bkgcolor.second,
        .b = scene.bkgcolor.third,
    };

    Ray ray = {
        .x = scene.eye.first,
        .y = scene.eye.second,
        .z = scene.eye.third,
        .dx = raydir.first,
        .dy = raydir.second,
        .dz = raydir.third,
    };

    int m_idx = -1; // material index
    // loop for all objects
    // check whether there is an intersection
    m_idx = intersect_check(scene, ray);
    if (m_idx >= 0) {
        res_color = shade_ray(scene, m_idx);
    }

    return res_color;
}
