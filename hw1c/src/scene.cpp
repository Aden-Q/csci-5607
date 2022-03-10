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
    int int_var[10];
    float float_var[10];
    char char_var[10];
    std::string line_buffer;
    int m_idx = -1;
    int obj_sphere_idx = 0;
    int obj_cylinder_idx = 0;
    int obj_vertex_idx = 1;  // only vertex index starts from 1
    int obj_triangle_idx = 0;
    int num_keywords = 0;

    while (inputstream >> keyword)
    {
        // check keywords
        if (keyword == "eye")
        {
            num_keywords++;
            // read the world coordinate of the eye
            inputstream >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 eye(float_var[0], float_var[1], float_var[2]);
            scene.eye = eye;
        }
        else if (keyword == "viewdir")
        {
            num_keywords++;
            // read the world coordinate of the view direction
            inputstream >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 viewdir(float_var[0], float_var[1], float_var[2]);
            scene.viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            num_keywords++;
            // read the world coordinate of the up direction
            inputstream >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 updir(float_var[0], float_var[1], float_var[2]);
            scene.updir = updir;
        }
        else if (keyword == "vfov")
        {
            num_keywords++;
            // read the vertical field of view (in degree)
            inputstream >> float_var[0];
            scene.vfov = float_var[0];
        }
        else if (keyword == "imsize")
        {
            num_keywords++;
            // read width and height of the image
            inputstream >> int_var[0] >> int_var[1];
            scene.width = int_var[0];
            scene.height = int_var[1];
        }
        else if (keyword == "bkgcolor")
        {
            num_keywords++;
            // read the background color
            inputstream >> float_var[0] >> float_var[1] >> float_var[2];
            Color bkgcolor(float_var[0], float_var[1], float_var[2]);
            scene.bkgcolor = bkgcolor;
        }
        else if (keyword == "light")
        {
            num_keywords++;
            // read in the light source
            inputstream >> float_var[0] >> float_var[1] >> float_var[2]
                        >> float_var[3] >> float_var[4] >> float_var[5]
                        >> float_var[6];
            Light light(float_var[0], float_var[1], float_var[2],
                        float_var[3], float_var[4], float_var[5],
                        float_var[6]);
            scene.light_list.push_back(light);
        }
        else if (keyword == "attlight")
        {
            num_keywords++;
            // read in the light source
            inputstream >> float_var[0] >> float_var[1] >> float_var[2] 
                        >> float_var[3] >> float_var[4] >> float_var[5] 
                        >> float_var[6] >> float_var[7] >> float_var[8]
                        >> float_var[9];
            AttLight attlight(float_var[0], float_var[1], float_var[2],
                              float_var[3], float_var[4], float_var[5],
                              float_var[6], float_var[7], float_var[8],
                              float_var[9]);
            scene.attlight_list.push_back(attlight);
        }
        else if (keyword == "depthcueing")
        {
            num_keywords++;
            // read in the light source
            inputstream >> float_var[0] >> float_var[1] >> float_var[2] 
                        >> float_var[3] >> float_var[4] >> float_var[5] 
                        >> float_var[6];
            DepthCueing depth_cue = {
                .dc_r = float_var[0],
                .dc_g = float_var[1],
                .dc_b = float_var[2],
                .alpha_max = float_var[3],
                .alpha_min = float_var[4],
                .dist_max = float_var[5],
                .dist_min = float_var[6]};
            scene.depth_cue = depth_cue;
            scene.depth_cue_enable = true;
        }
        else if (keyword == "mtlcolor")
        {
            // update the current material color
            m_idx++;
            inputstream >> float_var[0] >> float_var[1] >> float_var[2]
                        >> float_var[3] >> float_var[4] >> float_var[5]
                        >> float_var[6] >> float_var[7] >> float_var[8]
                        >> float_var[9];
            MtlColor material = {
                .Od_r = float_var[0],
                .Od_g = float_var[1],
                .Od_b = float_var[2],
                .Os_r = float_var[3],
                .Os_g = float_var[4],
                .Os_b = float_var[5],
                .ka = float_var[6],
                .kd = float_var[7],
                .ks = float_var[8],
                .n = float_var[9]};
            scene.material_list.push_back(material);
        }
        else if (keyword == "sphere")
        {
            // store parameters for the sphere
            inputstream >> float_var[0] >> float_var[1] >> float_var[2] >> float_var[3];
            Sphere sphere = {
                .obj_idx = obj_sphere_idx++,
                .m_idx = m_idx,
                .cx = float_var[0],
                .cy = float_var[1],
                .cz = float_var[2],
                .radius = float_var[3]};
            scene.sphere_list.push_back(sphere);
        }
        else if (keyword == "cylinder")
        {
            // store parameters for the cylinder
            inputstream >> float_var[0] >> float_var[1] >> float_var[2] 
                        >> float_var[3] >> float_var[4] >> float_var[5]
                        >> float_var[6] >> float_var[7];
            Cylinder cylinder = {
                .obj_idx = obj_cylinder_idx++,
                .m_idx = m_idx,
                .cx = float_var[0],
                .cy = float_var[1],
                .cz = float_var[2],
                .dx = float_var[3],
                .dy = float_var[4],
                .dz = float_var[5],
                .radius = float_var[6],
                .length = float_var[7]};
            scene.cylinder_list.push_back(cylinder);
        }
        else if (keyword == "v")
        {
            // store parameters for the vertex
            inputstream >> float_var[0] >> float_var[1] >> float_var[2];
            Vertex vertex = {
                .obj_idx = obj_vertex_idx++,
                .x = float_var[0],
                .y = float_var[1],
                .z = float_var[2]};
            scene.vertex_list.push_back(vertex);
        }
        else if (keyword == "f")
        {
            // store parameters for the triangle
            inputstream >> int_var[0] >> int_var[1] >> int_var[2];
            Triangle triangle = {
                .obj_idx = obj_triangle_idx++,
                .first = int_var[0],
                .second = int_var[1],
                .third = int_var[2]};
            scene.triangle_list.push_back(triangle);
        }
    }

    inputstream.close();
    return num_keywords;
}
