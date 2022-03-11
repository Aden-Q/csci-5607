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
    std::string line;
    int int_var[10];
    float float_var[10];
    int m_idx = -1;
    int obj_sphere_idx = 0;
    int obj_cylinder_idx = 0;
    int obj_vertex_idx = 1;  // only vertex index starts from 1
    int obj_vertex_normal_idx = 1;  // only vertex index starts from 1
    int obj_triangle_idx = 0;
    int num_keywords = 0;

    // read and process line by line
    while (std::getline(inputstream, line))
    {
        // initialize the keywork to empty
        keyword = "";
        // convert the string line to input string stream
        std::istringstream iss(line);
        iss >> keyword;
        // check keywords
        if (keyword == "eye")
        {
            num_keywords++;
            // read the world coordinate of the eye
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 eye(float_var[0], float_var[1], float_var[2]);
            scene.eye = eye;
        }
        else if (keyword == "viewdir")
        {
            num_keywords++;
            // read the world coordinate of the view direction
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 viewdir(float_var[0], float_var[1], float_var[2]);
            scene.viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            num_keywords++;
            // read the world coordinate of the up direction
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 updir(float_var[0], float_var[1], float_var[2]);
            scene.updir = updir;
        }
        else if (keyword == "vfov")
        {
            num_keywords++;
            // read the vertical field of view (in degree)
            iss >> float_var[0];
            scene.vfov = float_var[0];
        }
        else if (keyword == "imsize")
        {
            num_keywords++;
            // read width and height of the image
            iss >> int_var[0] >> int_var[1];
            scene.width = int_var[0];
            scene.height = int_var[1];
        }
        else if (keyword == "bkgcolor")
        {
            num_keywords++;
            // read the background color
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            Color bkgcolor(float_var[0], float_var[1], float_var[2]);
            scene.bkgcolor = bkgcolor;
        }
        else if (keyword == "light")
        {
            num_keywords++;
            // read in the light source
            iss >> float_var[0] >> float_var[1] >> float_var[2]
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
            iss >> float_var[0] >> float_var[1] >> float_var[2] 
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
            iss >> float_var[0] >> float_var[1] >> float_var[2] 
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
            iss >> float_var[0] >> float_var[1] >> float_var[2]
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
            iss >> float_var[0] >> float_var[1] >> float_var[2] >> float_var[3];
            Sphere sphere = {
                .obj_idx = obj_sphere_idx++,
                .m_idx = m_idx,
                .center = FloatVec3(float_var[0], float_var[1], float_var[2]),
                .radius = float_var[3]};
            scene.sphere_list.push_back(sphere);
        }
        else if (keyword == "cylinder")
        {
            // store parameters for the cylinder
            iss >> float_var[0] >> float_var[1] >> float_var[2] 
                        >> float_var[3] >> float_var[4] >> float_var[5]
                        >> float_var[6] >> float_var[7];
            Cylinder cylinder = {
                .obj_idx = obj_cylinder_idx++,
                .m_idx = m_idx,
                .center = FloatVec3(float_var[0], float_var[1], float_var[2]),
                .dir = FloatVec3(float_var[3], float_var[4], float_var[5]),
                .radius = float_var[6],
                .length = float_var[7]};
            scene.cylinder_list.push_back(cylinder);
        }
        else if (keyword == "v")
        {
            // store parameters for the vertex
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            Vertex vertex = {
                .obj_idx = obj_vertex_idx++,
                .p = FloatVec3(float_var[0], float_var[1], float_var[2])};
            scene.vertex_list.push_back(vertex);
        }
        else if (keyword == "f")
        {
            // check the input format specified by the user
            // whether it uses smooth shading, whether it uses texture mapping
            if (sscanf(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3)
            {
                // simple flat shading, use the format v
                // store parameters for the triangle
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .v0 = scene.vertex_list[int_var[0] - 1],
                    .v1 = scene.vertex_list[int_var[1] - 1],
                    .v2 = scene.vertex_list[int_var[2] - 1],
                    .smooth_shade = false,
                    .texture_map = false};
                scene.triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5) == 6)
            {
                // smooth shading, use the format v//vn
                // store parameters for the triangle
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .v0 = scene.vertex_list[int_var[0] - 1],
                    .v1 = scene.vertex_list[int_var[1] - 1],
                    .v2 = scene.vertex_list[int_var[2] - 1],
                    .smooth_shade = true,
                    .vn0_idx = int_var[3],
                    .vn1_idx = int_var[4],
                    .vn2_idx = int_var[5],
                    .texture_map = false};
                scene.triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5) == 6)
            {
                // texture mapping, use the format v/t
            }
            else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5,
                            int_var + 6, int_var + 7, int_var + 8) == 9)
            {
                // smooth shading, and texture mapping, use the format v/vt/vn
            }
        }
        else if (keyword == "vn")
        {
            // store parameters for the vertex normal
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            VertexNormal vertex_normal = {
                .obj_idx = obj_vertex_normal_idx++,
                .n = FloatVec3(float_var[0], float_var[1], float_var[2]).normal()};
            scene.vertex_normal_list.push_back(vertex_normal);
        }
    }

    inputstream.close();
    return num_keywords;
}
