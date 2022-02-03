/**
 * @file ray_casting.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "types.h"
#include "ray.h"
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
    float mtl_r, mtl_g, mtl_b;  // keep track of the current material color
    
    while (std::getline(inputstream, keyword, ' '))
    {
        // check keywords
        if (keyword == "eye") {
            // read the world coordinate of the eye
            inputstream >> a >> b >> c;
            scene.eye_x = std::stof(a);
            scene.eye_y = std::stof(b);
            scene.eye_z = std::stof(c);
        } else if (keyword == "viewdir") {
            // read the world coordinate of the view direction
            inputstream >> a >> b >> c;
            scene.vdir_x = std::stof(a);
            scene.vdir_y = std::stof(b);
            scene.vdir_z = std::stof(c);
        } else if (keyword == "updir") {
            // read the world coordinate of the up direction
            inputstream >> a >> b >> c;
            scene.up_x = std::stof(a);
            scene.up_y = std::stof(b);
            scene.up_z = std::stof(c);
        } else if (keyword == "vfov") {
            // read the vertical field of view (in degree)
            inputstream >> a;
            scene.vfov = std::stof(a);
        } else if (keyword == "imsize") {
            // read width and height of the image
            inputstream >> a >> b;
            scene.width = std::stof(a);
            scene.height = std::stof(b);
        } else if (keyword == "bkgcolor") {
            // read the background color
            inputstream >> a >> b >> c;
            scene.bkg_r = std::stof(a);
            scene.bkg_g = std::stof(b);
            scene.bkg_b = std::stof(c);
        } else if (keyword == "mtlcolor") {
            // update the current material color
            inputstream >> a >> b >> c;
            mtl_r = std::stof(a);
            mtl_g = std::stof(b);
            mtl_b = std::stof(c);
        } else if (keyword == "sphere") {
            // rendering the sphere with the current material color
            // store the spacial location and radius of the sphere
            continue;
        }
    }

    // std::cout << std::setprecision(10) << scene.eye_x << " " << scene.eye_y << " " << scene.eye_z << std::endl;
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect number of arguments! Usage: ./raytracer filename");
        exit(-1);
    }
    // instantiate types
    RayType ray;
    Scene scene;

    std::string filename = argv[1];
    parse_scene(filename, scene);

    return 0;
}
