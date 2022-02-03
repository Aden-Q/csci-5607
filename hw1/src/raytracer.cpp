/**
 * @file ray_casting.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <string>
#include <vector>
#include "types.h"
#include "ray.h"
#include "utils.h"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect number of arguments! Usage: ./raytracer filename");
        exit(-1);
    }
    // instantiate ray, scene and image
    RayType ray;
    Scene scene;
    // Image checkerboard[MAX_WIDTH][MAX_HEIGHT];

    // get the filename from command line args and parse the file
    std::string filename = argv[1];
    parse_scene(filename, scene);

    // dynamically allocate a 2d array to store pixels in the image
    Image **checkerboard = new Image*[scene.height];
    for (int i = 0; i < scene.width; i++) {
        checkerboard[i] = new Image[scene.width];
    }


    return 0;
}
