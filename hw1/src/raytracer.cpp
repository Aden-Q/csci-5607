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
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments! Usage: ./raytracer filename\n");
        exit(-1);
    }
    // instantiate ray, scene and image
    Scene scene;
    ViewWindow viewwindow;
    float viewdist = 5;
    int num_keywords = 0;

    // get the filename from command line args and parse the file
    std::string filename = argv[1];
    num_keywords = parse_scene(filename, scene);
    if (num_keywords < 6) {
        fprintf(stderr, "Missing some keywords! Pleaze recheck your input file!\n");
        exit(-1);
    }

    // calculate viewwindow parameters, giving a chosen viewing distance
    view_window_init(scene, viewwindow, viewdist);

    // dynamically allocate a 2d array to store pixels in the image
    Image **checkerboard = new Image*[scene.width];
    for (int i = 0; i < scene.width; i++) {
        checkerboard[i] = new Image[scene.height];
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

    output_image(filename + "_raytracer.ppm", checkerboard, scene);

    return 0;
}
