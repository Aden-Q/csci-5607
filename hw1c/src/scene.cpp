/**
 * @file scene.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "scene.h"
#include "utils.h"

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
    char str_var[10][50];
    int m_idx = -1;
    int texture_idx = -1;
    int obj_sphere_idx = 0;
    int obj_cylinder_idx = 0;
    int obj_vertex_idx = 1;  // vertex index starts from 1
    int obj_vertex_normal_idx = 1;  // vertex normal index starts from 1
    int obj_texture_coordinate_idx = 1;  // texture coordinate index starts from 1
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
                .Od_lambda = Color(float_var[0], float_var[1], float_var[2]),
                .Os_lambda = Color(float_var[3], float_var[4], float_var[5]),
                .ka = float_var[6],
                .kd = float_var[7],
                .ks = float_var[8],
                .n = float_var[9]};
            scene.material_list.push_back(material);
        }
        else if (keyword == "texture")
        {
            // read in the filename for the texture file
            if (sscanf(line.c_str(), "texture %s", str_var[0]) == 1)
            {
                // update the current material color
                texture_idx++;
                // open the texture image and bind it to a input stream
                std::ifstream texture_inputstream(str_var[0], std::ios::in | std::ios::binary);
                // read the header information
                texture_inputstream >> str_var[1] >> int_var[0] >> int_var[1] >> int_var[2];
                Texture texture;
                texture.width = int_var[0];
                texture.height = int_var[1];
                texture.max_val = int_var[2];
                // initialize the checkerboard
                texture.checkerboard = new Color *[texture.width];
                for (int i = 0; i < texture.width; i++)
                {
                    texture.checkerboard[i] = new Color[texture.height];
                }
                for (int j = 0; j < texture.height; j++)
                {
                    for (int i = 0; i < texture.width; i++)
                    {
                        texture_inputstream >> int_var[3] >> int_var[4] >> int_var[5];
                        texture.checkerboard[i][j].r = int_var[3] * 1.0 / MAX_VAL;
                        texture.checkerboard[i][j].g = int_var[4] * 1.0 / MAX_VAL;
                        texture.checkerboard[i][j].b = int_var[5] * 1.0 / MAX_VAL;
                    }
                }
                scene.texture_list.push_back(texture);
            }
        }
        else if (keyword == "sphere")
        {
            // store parameters for the sphere
            iss >> float_var[0] >> float_var[1] >> float_var[2] >> float_var[3];
            Sphere sphere = {
                .obj_idx = obj_sphere_idx++,
                .m_idx = m_idx,
                .texture_idx = texture_idx,
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
                .texture_idx = texture_idx,
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
        else if (keyword == "vn")
        {
            // store parameters for the vertex normal
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            VertexNormal vertex_normal = {
                .obj_idx = obj_vertex_normal_idx++,
                .n = FloatVec3(float_var[0], float_var[1], float_var[2]).normal()};
            scene.vertex_normal_list.push_back(vertex_normal);
        }
        else if (keyword == "vt")
        {
            // store parameters for the texture coordinate
            iss >> float_var[0] >> float_var[1];
            TextureCoordinate texture_coordinate = {
                .obj_idx = obj_texture_coordinate_idx++,
                .vt = FloatVec2(float_var[0], float_var[1])};
            scene.texture_coordinate_list.push_back(texture_coordinate);
        }
        else if (keyword == "f")
        {
            // check the input format specified by the user
            // whether it uses smooth shading, whether it uses texture mapping
            if (sscanf(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3)
            {
                // simple flat shading, use the format v
                // smooth shading not applied
                // texture mappign not applied
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .texture_idx = -1,
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
                // only smooth shading enabled, use the format v//vn
                // smooth shading applied
                // texture mappign not applied
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .texture_idx = -1,
                    .v0 = scene.vertex_list[int_var[0] - 1],
                    .v1 = scene.vertex_list[int_var[2] - 1],
                    .v2 = scene.vertex_list[int_var[4] - 1],
                    .smooth_shade = true,
                    .vn0_idx = int_var[1],
                    .vn1_idx = int_var[3],
                    .vn2_idx = int_var[5],
                    .texture_map = false};
                scene.triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5) == 6)
            {
                // only texture mapping enabled, use the format v/vt
                // smooth shading not applied
                // texture mapping applied
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .texture_idx = texture_idx,
                    .v0 = scene.vertex_list[int_var[0] - 1],
                    .v1 = scene.vertex_list[int_var[2] - 1],
                    .v2 = scene.vertex_list[int_var[4] - 1],
                    .smooth_shade = false,
                    .texture_map = true,
                    .vt0_idx = int_var[1],
                    .vt1_idx = int_var[3],
                    .vt2_idx = int_var[5]};
                scene.triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5,
                            int_var + 6, int_var + 7, int_var + 8) == 9)
            {
                // smooth shading, and texture mapping, use the format v/vt/vn
                Triangle triangle = {
                    .obj_idx = obj_triangle_idx++,
                    .m_idx = m_idx,
                    .texture_idx = texture_idx,
                    .v0 = scene.vertex_list[int_var[0] - 1],
                    .v1 = scene.vertex_list[int_var[3] - 1],
                    .v2 = scene.vertex_list[int_var[6] - 1],
                    .smooth_shade = true,
                    .vn0_idx = int_var[1],
                    .vn1_idx = int_var[4],
                    .vn2_idx = int_var[7],
                    .texture_map = true,
                    .vt0_idx = int_var[2],
                    .vt1_idx = int_var[5],
                    .vt2_idx = int_var[8]};
                scene.triangle_list.push_back(triangle);
            }
        }
    }

    inputstream.close();
    return num_keywords;
}
