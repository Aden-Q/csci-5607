/**
 * @file utils.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include "types.h"

void parse_scene(std::string filename, Scene &scene)
{
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);

    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string keyword;
    std::string a, b, c, d;
    float mtl_r, mtl_g, mtl_b; // keep track of the current material color

    while (inputstream >> keyword)
    {
        // check keywords
        if (keyword == "eye")
        {
            // read the world coordinate of the eye
            inputstream >> a >> b >> c;
            scene.eye_x = std::stof(a);
            scene.eye_y = std::stof(b);
            scene.eye_z = std::stof(c);
        }
        else if (keyword == "viewdir")
        {
            // read the world coordinate of the view direction
            inputstream >> a >> b >> c;
            scene.vdir_x = std::stof(a);
            scene.vdir_y = std::stof(b);
            scene.vdir_z = std::stof(c);
        }
        else if (keyword == "updir")
        {
            // read the world coordinate of the up direction
            inputstream >> a >> b >> c;
            scene.up_x = std::stof(a);
            scene.up_y = std::stof(b);
            scene.up_z = std::stof(c);
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
            scene.width = std::stof(a);
            scene.height = std::stof(b);
        }
        else if (keyword == "bkgcolor")
        {
            // read the background color
            inputstream >> a >> b >> c;
            scene.bkg_r = std::stof(a);
            scene.bkg_g = std::stof(b);
            scene.bkg_b = std::stof(c);
        }
        else if (keyword == "mtlcolor")
        {
            // update the current material color
            inputstream >> a >> b >> c;
            mtl_r = std::stof(a);
            mtl_g = std::stof(b);
            mtl_b = std::stof(c);
        }
        else if (keyword == "sphere")
        {
            // rendering the sphere with the current material color
            // store the spacial location and radius of the sphere
            inputstream >> a >> b >> c >> d;
            Sphere sphere = {
                .r = mtl_r,
                .g = mtl_g,
                .b = mtl_b,
                .c_x = std::stof(a),
                .c_y = std::stof(b),
                .c_z = std::stof(c),
                .radius = std::stof(d)};
            scene.sphere_list.push_back(sphere);
        }
    }

    return;
}
