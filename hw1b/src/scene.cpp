/**
 * @file scene.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "scene.h"

int parse_scene(std::string filename, Scene &scene)
{
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);

    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string keyword;
    std::string str_param[10];
    int m_idx = -1;
    int obj_sphere_idx = 0;
    int obj_cylinder_idx = 0;
    int num_keywords = 0;

    while (inputstream >> keyword)
    {
        // check keywords
        if (keyword == "eye")
        {
            num_keywords++;
            // read the world coordinate of the eye
            inputstream >> str_param[0] >> str_param[1] >> str_param[2];
            FloatVec3 eye(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
            scene.eye = eye;
        }
        else if (keyword == "viewdir")
        {
            num_keywords++;
            // read the world coordinate of the view direction
            inputstream >> str_param[0] >> str_param[1] >> str_param[2];
            FloatVec3 viewdir(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
            scene.viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            num_keywords++;
            // read the world coordinate of the up direction
            inputstream >> str_param[0] >> str_param[1] >> str_param[2];
            FloatVec3 updir(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
            scene.updir = updir;
        }
        else if (keyword == "vfov")
        {
            num_keywords++;
            // read the vertical field of view (in degree)
            inputstream >> str_param[0];
            scene.vfov = std::stof(str_param[0]);
        }
        else if (keyword == "imsize")
        {
            num_keywords++;
            // read width and height of the image
            inputstream >> str_param[0] >> str_param[1];
            scene.width = std::stoi(str_param[0]);
            scene.height = std::stoi(str_param[1]);
        }
        else if (keyword == "bkgcolor")
        {
            num_keywords++;
            // read the background color
            inputstream >> str_param[0] >> str_param[1] >> str_param[2];
            FloatVec3 bkgcolor(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
            scene.bkgcolor = bkgcolor;
        }
        else if (keyword == "light")
        {
            num_keywords++;
            // read in the light source
            inputstream >> str_param[0] >> str_param[1] >> str_param[2]
                        >> str_param[3] >> str_param[4] >> str_param[5]
                        >> str_param[6];
            Light light = {
                .x = std::stof(str_param[0]),
                .y = std::stof(str_param[1]),
                .z = std::stof(str_param[2]),
                .w = std::stof(str_param[3]),
                .r = std::stof(str_param[4]),
                .g = std::stof(str_param[5]),
                .b = std::stof(str_param[6])};
            scene.light_list.push_back(light);
        }
        else if (keyword == "mtlcolor")
        {
            // update the current material color
            m_idx++;
            inputstream >> str_param[0] >> str_param[1] >> str_param[2]
                        >> str_param[3] >> str_param[4] >> str_param[5]
                        >> str_param[6] >> str_param[7] >> str_param[8]
                        >> str_param[9];
            MtlColor material = {
                .Od_r = std::stof(str_param[0]),
                .Od_g = std::stof(str_param[1]),
                .Od_b = std::stof(str_param[2]),
                .Os_r = std::stof(str_param[3]),
                .Os_g = std::stof(str_param[4]),
                .Os_b = std::stof(str_param[5]),
                .ka = std::stof(str_param[6]),
                .kd = std::stof(str_param[7]),
                .ks = std::stof(str_param[8]),
                .n = std::stof(str_param[9])};
            scene.material_list.push_back(material);
        }
        else if (keyword == "sphere")
        {
            // store parameters for the sphere
            inputstream >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3];
            Sphere sphere = {
                .obj_idx = obj_sphere_idx++,
                .m_idx = m_idx,
                .cx = std::stof(str_param[0]),
                .cy = std::stof(str_param[1]),
                .cz = std::stof(str_param[2]),
                .radius = std::stof(str_param[3])};
            scene.sphere_list.push_back(sphere);
        }
        else if (keyword == "cylinder")
        {
            // store parameters for the cylinder
            inputstream >> str_param[0] >> str_param[1] >> str_param[2] 
                        >> str_param[3] >> str_param[4] >> str_param[5]
                        >> str_param[6] >> str_param[7];
            Cylinder cylinder = {
                .obj_idx = obj_cylinder_idx++,
                .m_idx = m_idx,
                .cx = std::stof(str_param[0]),
                .cy = std::stof(str_param[1]),
                .cz = std::stof(str_param[2]),
                .dx = std::stof(str_param[3]),
                .dy = std::stof(str_param[4]),
                .dz = std::stof(str_param[5]),
                .radius = std::stof(str_param[6]),
                .length = std::stof(str_param[7])};
            scene.cylinder_list.push_back(cylinder);
        }
    }

    inputstream.close();
    return num_keywords;
}
