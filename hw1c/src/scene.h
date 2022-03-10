/**
 * @file scene.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_

// constant
#define PI 3.14159265

#include <iostream>
#include <fstream>
#include <string>
#include "types.h"

// parse the scene parameters from the input file, return the number of keywords catched
int parse_scene(std::string filename, Scene &scene);

#endif // SRC_SCENE_H_
