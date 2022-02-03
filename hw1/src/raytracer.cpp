/**
 * @file ray_casting.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "types.h"
#include "utils.h"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect number of arguments! Usage: ./raytracer filename");
        exit(-1);
    }
    // instantiate ray, scene and image
    Scene scene;
    ViewWindow viewwindow;
    float viewdist = 5;
    // Image checkerboard[MAX_WIDTH][MAX_HEIGHT];

    // get the filename from command line args and parse the file
    std::string filename = argv[1];
    parse_scene(filename, scene);

    // calculate viewwindow parameters, giving a chosen viewing distance
    view_window_init(scene, viewwindow, viewdist);

    // dynamically allocate a 2d array to store pixels in the image
    Image **checkerboard = new Image*[scene.height];
    for (int i = 0; i < scene.width; i++) {
        checkerboard[i] = new Image[scene.width];
    }
    // run ray tracing and assign a color for each pixel
    for (int i = 0; i < scene.width; i++) {
        for (int j = 0; j < scene.height; j++) {
            Color res_color = trace_ray(scene, viewwindow, i, j);
            checkerboard[i][j].r = res_color.r * 255;
            checkerboard[i][j].g = res_color.g * 255;
            checkerboard[i][j].b = res_color.b * 255;
        }
    }

    // placeholder
    // for
    // std::cout << viewwindow.v.first << std::endl;
    // std::cout << viewwindow.v.second << std::endl;
    // std::cout << viewwindow.v.third << std::endl;
    // std::cout << viewwindow.width << std::endl;
    // FloatVec3 new_c(viewwindow.ul + viewwindow.dh * 33 + viewwindow.dv * 66);
    // std::cout << new_c.first << std::endl;
    // std::cout << new_c.second << std::endl;
    // std::cout << new_c.third << std::endl;

    output_image("raytracer.ppm", checkerboard, scene);

    return 0;
}
