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
#include "types.h"
#include "utils.h"

int parse_scene(std::string filename, Scene &scene)
{
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);

    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string keyword;
    std::string a, b, c, d, e, f, g, h;
    int m_idx = -1;
    int num_keywords = 0;

    while (inputstream >> keyword)
    {
        // check keywords
        if (keyword == "eye")
        {
            num_keywords++;
            // read the world coordinate of the eye
            inputstream >> a >> b >> c;
            FloatVec3 eye(std::stof(a), std::stof(b), std::stof(c));
            scene.eye = eye;
        }
        else if (keyword == "viewdir")
        {
            num_keywords++;
            // read the world coordinate of the view direction
            inputstream >> a >> b >> c;
            FloatVec3 viewdir(std::stof(a), std::stof(b), std::stof(c));
            scene.viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            num_keywords++;
            // read the world coordinate of the up direction
            inputstream >> a >> b >> c;
            FloatVec3 updir(std::stof(a), std::stof(b), std::stof(c));
            scene.updir = updir;
        }
        else if (keyword == "vfov")
        {
            num_keywords++;
            // read the vertical field of view (in degree)
            inputstream >> a;
            scene.vfov = std::stof(a);
        }
        else if (keyword == "imsize")
        {
            num_keywords++;
            // read width and height of the image
            inputstream >> a >> b;
            scene.width = std::stoi(a);
            scene.height = std::stoi(b);
        }
        else if (keyword == "bkgcolor")
        {
            num_keywords++;
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
                .b = std::stof(c)
                };
            scene.material_list.push_back(material);
        }
        else if (keyword == "sphere")
        {
            // store parameters for the sphere
            inputstream >> a >> b >> c >> d;
            Sphere sphere = {
                .m_idx = m_idx,
                .cx = std::stof(a),
                .cy = std::stof(b),
                .cz = std::stof(c),
                .radius = std::stof(d)
                };
            scene.sphere_list.push_back(sphere);
        }
        else if (keyword == "cylinder")
        {
            // store parameters for the cylinder
            inputstream >> a >> b >> c >> d >> e >> f >> g >> h;
            Cylinder cylinder = {
                .m_idx = m_idx,
                .cx = std::stof(a),
                .cy = std::stof(b),
                .cz = std::stof(c),
                .dx = std::stof(d),
                .dy = std::stof(e),
                .dz = std::stof(f),
                .radius = std::stof(g),
                .length = std::stof(h)
                };
            scene.cylinder_list.push_back(cylinder);
        }
    }

    inputstream.close();
    return num_keywords;
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

Color shade_ray(const Scene &scene, float m_idx) 
{
    // return the corresponding color for that object
    return scene.material_list[m_idx];
}

int intersect_check(const Scene &scene, Ray &ray)
{
    float min_t = 100000;
    float temp_t, temp_x, temp_y, temp_z;
    int res_idx = -1;
    float A;
    float B;
    float C;
    float determinant;

    // check intersects for spheres
    for (auto s:scene.sphere_list) 
    {
        B = 2 * (ray.dx * (ray.x - s.cx) + ray.dy * (ray.y - s.cy) + ray.dz * (ray.z - s.cz));
        C = pow(ray.x - s.cx, 2) + pow(ray.y - s.cy, 2) + pow(ray.z - s.cz, 2) - pow(s.radius, 2);
        determinant = pow(B, 2) - 4 * C;
        if (determinant > -1e-6)  // greater than or equal to 0
        {   // need further check
            temp_t = (-B - sqrt(determinant)) / 2;
            if (temp_t > 0 && temp_t < min_t) 
            {
                min_t = temp_t;
                res_idx = s.m_idx;
            }
            // check for another possible solution
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 0 && temp_t < min_t)
            {
                min_t = temp_t;
                res_idx = s.m_idx;
            }
        }
    }

    // check intersects for cylinders
    for (auto c:scene.cylinder_list) 
    {
        // std::cout << ray.x << " " << ray.y << std::endl;
        // consider three cases: three possible directions for the cylinder
        if (c.dx > 1e-6) // the cylinder heads towards positive x axis
        {
            // check for side surface
            A = 1 - pow(ray.dx, 2);  // A is not equal to 0 in this case
            B = 2 * (ray.dy * (ray.y - c.cy) + ray.dz * (ray.z - c.cz));
            C = pow(ray.y - c.cy, 2) + pow(ray.z - c.cz, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {                        // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.x + temp_t * ray.dx, c.cx - c.length / 2, c.cx + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for two base surfaces
            temp_t = (c.cx - c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 0)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for another surface
            temp_t = (c.cx + c.length / 2 - ray.x) / ray.dx;
            if (temp_t > 0)
            {
                temp_y = ray.y + temp_t * ray.dy;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_y, temp_z);
                FloatVec2 p2(c.cy, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
        }
        else if (c.dy > 1e-6) // the cylinder heads towards positive y axis
        {
            A = 1 - pow(ray.dy, 2); // A is not equal to 0 in this case
            B = 2 * (ray.dx * (ray.x - c.cx) + ray.dz * (ray.z - c.cz));
            C = pow(ray.x - c.cx, 2) + pow(ray.z - c.cz, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {   // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.y + temp_t * ray.dy, c.cy - c.length / 2, c.cy + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for two base surfaces
            temp_t = (c.cy - c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for another surface
            temp_t = (c.cy + c.length / 2 - ray.y) / ray.dy;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_z = ray.z + temp_t * ray.dz;
                FloatVec2 p1(temp_x, temp_z);
                FloatVec2 p2(c.cx, c.cz);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
        }
        else if (c.dz > 1e-6) // the cylinder heads towards positive z axis
        {
            A = 1 - pow(ray.dz, 2); // A is not equal to 0 in this case
            B = 2 * (ray.dx * (ray.x - c.cx) + ray.dy * (ray.y - c.cy));
            C = pow(ray.x - c.cx, 2) + pow(ray.y - c.cy, 2) - pow(c.radius, 2);
            determinant = pow(B, 2) - 4 * A * C;
            if (determinant > 0) // greater than or equal to 0
            {                    // need further check
                temp_t = (-B - sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
                // check for another possible solution
                temp_t = (-B + sqrt(determinant)) / (2 * A);
                // check whether the solution point lies on the cylinder
                if (temp_t > 0 && temp_t < min_t && lie_within(ray.z + temp_t * ray.dz, c.cz - c.length / 2, c.cz + c.length / 2))
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for two base surfaces
            temp_t = (c.cz - c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
            // check for another surface
            temp_t = (c.cz + c.length / 2 - ray.z) / ray.dz;
            if (temp_t > 0)
            {
                temp_x = ray.x + temp_t * ray.dx;
                temp_y = ray.y + temp_t * ray.dy;
                FloatVec2 p1(temp_x, temp_y);
                FloatVec2 p2(c.cx, c.cy);
                if (temp_t < min_t && distance_between_2D_points(p1, p2) < c.radius)
                {
                    min_t = temp_t;
                    res_idx = c.m_idx;
                }
            }
        }
        else
        {
            // placeholder here
            // don't need to consider other possible directions for assignment 1a
        }
    }

    return res_idx;
}

Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h) 
{
    // get a ray representation
    FloatVec3 point_in_view(viewwindow.ul + viewwindow.dh * w + viewwindow.dv * h);
    FloatVec3 raydir = vector_normalize(point_in_view - scene.eye);
    // initialize the response color to be the background color
    Color res_color = 
    {
        .r = scene.bkgcolor.first,
        .g = scene.bkgcolor.second,
        .b = scene.bkgcolor.third,
    };

    Ray ray = 
    {
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
    if (m_idx >= 0) 
    {
        res_color = shade_ray(scene, m_idx);
    }

    return res_color;
}
