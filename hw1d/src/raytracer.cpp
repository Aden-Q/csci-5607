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
#include "scene.h"
#include "ray.h"


int main(int argc, char **argv)
{
    if (argc != 2) 
    {
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
    num_keywords = scene.parseScene(filename);
    if (num_keywords < 7) 
    {
        fprintf(stderr, "Missing some keywords! Pleaze recheck your input file!\n");
        exit(-1);
    }

    // calculate viewwindow parameters, giving a chosen viewing distance
    view_window_init(scene, viewwindow, viewdist);

    // dynamically allocate a 2d array to store pixels in the image
    Color **checkerboard = new Color *[scene.getWidth()];
    for (int i = 0; i < scene.getWidth(); i++) 
    {
        checkerboard[i] = new Color[scene.getHeight()];
    }
    // run ray tracing and assign a color for each pixel
    for (int j = 0; j < scene.getHeight(); j++)
    {
        for (int i = 0; i < scene.getWidth(); i++) 
        {
            Color res_color = trace_ray(scene, viewwindow, i, j);
            checkerboard[i][j].setR(res_color.getR());
            checkerboard[i][j].setG(res_color.getG());
            checkerboard[i][j].setB(res_color.getB());
        }
    }

    // produce a final image
    output_image(filename + ".ppm", checkerboard, scene.getWidth(), scene.getHeight());
    return 0;
}
