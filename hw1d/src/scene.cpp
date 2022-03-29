/**
 * @file scene.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "scene.h"
#include "utils.h"

Scene::Scene()
{
    this->eye = FloatVec3(0, 0, 0);
    this->viewdir = FloatVec3(0, 0, 1);
    this->updir = FloatVec3(0, 0, 1);
    this->bkgcolor = Color(1, 1, 1);
    this->vfov = 90;
    this->width = 512;
    this->height = 512;
    this->depth_cue_enable = false;
}

int Scene::parseScene(std::string filename)
{
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);

    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string keyword;
    std::string line;
    int int_var[20];
    float float_var[20];
    char str_var[10][50];
    int m_idx = -1;
    int texture_idx = -1;
    int bump_idx = -1;
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
            this->eye = eye;
        }
        else if (keyword == "viewdir")
        {
            num_keywords++;
            // read the world coordinate of the view direction
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 viewdir(float_var[0], float_var[1], float_var[2]);
            this->viewdir = viewdir;
        }
        else if (keyword == "updir")
        {
            num_keywords++;
            // read the world coordinate of the up direction
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            FloatVec3 updir(float_var[0], float_var[1], float_var[2]);
            this->updir = updir;
        }
        else if (keyword == "vfov")
        {
            num_keywords++;
            // read the vertical field of view (in degree)
            iss >> float_var[0];
            this->vfov = float_var[0];
        }
        else if (keyword == "imsize")
        {
            num_keywords++;
            // read width and height of the image
            iss >> int_var[0] >> int_var[1];
            this->width = int_var[0];
            this->height = int_var[1];
        }
        else if (keyword == "bkgcolor")
        {
            num_keywords++;
            // read the background color
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            Color bkgcolor(float_var[0], float_var[1], float_var[2]);
            this->bkgcolor = bkgcolor;
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
            this->light_list.push_back(light);
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
            this->attlight_list.push_back(attlight);
        }
        else if (keyword == "depthcueing")
        {
            num_keywords++;
            // read in the light source
            iss >> float_var[0] >> float_var[1] >> float_var[2] 
                >> float_var[3] >> float_var[4] >> float_var[5] 
                >> float_var[6];
            DepthCue depth_cue = {
                .dc_r = float_var[0],
                .dc_g = float_var[1],
                .dc_b = float_var[2],
                .alpha_max = float_var[3],
                .alpha_min = float_var[4],
                .dist_max = float_var[5],
                .dist_min = float_var[6]};
            this->depth_cue = depth_cue;
            this->depth_cue_enable = true;
        }
        else if (keyword == "mtlcolor")
        {
            num_keywords++;
            // update the current material color
            m_idx++;
            iss >> float_var[0] >> float_var[1] >> float_var[2]
                >> float_var[3] >> float_var[4] >> float_var[5]
                >> float_var[6] >> float_var[7] >> float_var[8]
                >> float_var[9] >> float_var[10] >> float_var[11];
            MaterialColor material(Color(float_var[0], float_var[1], float_var[2]),
                                   Color(float_var[3], float_var[4], float_var[5]),
                                   float_var[6],
                                   float_var[7],
                                   float_var[8],
                                   float_var[9],
                                   float_var[10],
                                   float_var[11]);
            this->material_list.push_back(material);
        }
        else if (keyword == "texture")
        {
            num_keywords++;
            // read in the filename for the texture file
            if (sscanf(line.c_str(), "texture %s", str_var[0]) == 1)
            {
                // update the current material color
                texture_idx++;
                // open the texture image and bind it to a input stream
                std::ifstream texture_inputstream(str_var[0], std::ios::in | std::ios::binary);
                // read the header information
                texture_inputstream >> str_var[1] >> int_var[0] >> int_var[1] >> int_var[2];
                Texture texture(int_var[0], int_var[1], int_var[2]);
                Color **checkerboard = texture.getCheckerboard();
                for (int j = 0; j < texture.getHeight(); j++)
                {
                    for (int i = 0; i < texture.getWidth(); i++)
                    {
                        texture_inputstream >> int_var[3] >> int_var[4] >> int_var[5];
                        checkerboard[i][j].setR(int_var[3] * 1.0 / texture.getMaxVal());
                        checkerboard[i][j].setG(int_var[4] * 1.0 / texture.getMaxVal());
                        checkerboard[i][j].setB(int_var[5] * 1.0 / texture.getMaxVal());
                    }
                }
                this->texture_list.push_back(texture);
            }
        }
        else if (keyword == "bump")
        {
            num_keywords++;
            // read in the filename for the normal map
            if (sscanf(line.c_str(), "bump %s", str_var[0]) == 1)
            {
                // update the current material color
                bump_idx++;
                // open the texture image and bind it to a input stream
                std::ifstream bump_inputstream(str_var[0], std::ios::in | std::ios::binary);
                // read the header information
                bump_inputstream >> str_var[1] >> int_var[0] >> int_var[1] >> int_var[2];
                Bump bump(int_var[0], int_var[1], int_var[2]);
                FloatVec3 **checkerboard = bump.getCheckerboard();
                for (int j = 0; j < bump.getHeight(); j++)
                {
                    for (int i = 0; i < bump.getWidth(); i++)
                    {
                        bump_inputstream >> int_var[3] >> int_var[4] >> int_var[5];
                        checkerboard[i][j].first = (int_var[3] * 1.0 / bump.getMaxVal() * 2 - 1);
                        checkerboard[i][j].second = (int_var[4] * 1.0 / bump.getMaxVal() * 2 - 1);
                        checkerboard[i][j].third = (int_var[5] * 1.0 / bump.getMaxVal() * 2 - 1);
                    }
                }
                this->bump_list.push_back(bump);
            }
        }
        else if (keyword == "sphere")
        {
            num_keywords++;
            // store parameters for the sphere
            iss >> float_var[0] >> float_var[1] >> float_var[2] >> float_var[3];
            Sphere sphere(obj_sphere_idx++, m_idx, texture_idx, bump_idx,
                          FloatVec3(float_var[0], float_var[1], float_var[2]),
                          float_var[3]);
            this->sphere_list.push_back(sphere);
        }
        else if (keyword == "cylinder")
        {
            num_keywords++;
            // store parameters for the cylinder
            iss >> float_var[0] >> float_var[1] >> float_var[2] 
                        >> float_var[3] >> float_var[4] >> float_var[5]
                        >> float_var[6] >> float_var[7];
            Cylinder cylinder(obj_cylinder_idx++,
                              m_idx,
                              texture_idx,
                              bump_idx,
                              FloatVec3(float_var[0], float_var[1], float_var[2]),
                              FloatVec3(float_var[3], float_var[4], float_var[5]),
                              float_var[6],
                              float_var[7]);
            this->cylinder_list.push_back(cylinder);
        }
        else if (keyword == "v")
        {
            num_keywords++;
            // store parameters for the vertex
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            Vertex vertex = {
                .obj_idx = obj_vertex_idx++,
                .p = FloatVec3(float_var[0], float_var[1], float_var[2])};
            this->vertex_list.push_back(vertex);
        }
        else if (keyword == "vn")
        {
            num_keywords++;
            // store parameters for the vertex normal
            iss >> float_var[0] >> float_var[1] >> float_var[2];
            VertexNormal vertex_normal = {
                .obj_idx = obj_vertex_normal_idx++,
                .n = FloatVec3(float_var[0], float_var[1], float_var[2]).normal()};
            this->vertex_normal_list.push_back(vertex_normal);
        }
        else if (keyword == "vt")
        {
            num_keywords++;
            // store parameters for the texture coordinate
            iss >> float_var[0] >> float_var[1];
            TextureCoordinate texture_coordinate = {
                .obj_idx = obj_texture_coordinate_idx++,
                .vt = FloatVec2(float_var[0], float_var[1])};
            this->texture_coordinate_list.push_back(texture_coordinate);
        }
        else if (keyword == "f")
        {
            num_keywords++;
            // check the input format specified by the user
            // whether it uses smooth shading, whether it uses texture mapping
            if (sscanf(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3)
            {
                // simple flat shading, use the format v
                // smooth shading not applied
                // texture mappign not applied
                Triangle triangle;
                triangle.setID(obj_triangle_idx++);
                triangle.setMidx(m_idx);
                triangle.setTextureidx(-1);
                triangle.setBumpidx(bump_idx);
                triangle.setV0idx(int_var[0]);
                triangle.setV1idx(int_var[1]);
                triangle.setV2idx(int_var[2]);
                this->triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5) == 6)
            {
                // only smooth shading enabled, use the format v//vn
                // smooth shading applied
                // texture mappign not applied
                Triangle triangle;
                triangle.setID(obj_triangle_idx++);
                triangle.setMidx(m_idx);
                triangle.setTextureidx(-1);
                triangle.setBumpidx(bump_idx);
                triangle.setV0idx(int_var[0]);
                triangle.setV1idx(int_var[2]);
                triangle.setV2idx(int_var[4]);
                triangle.setSmoothShade(true);
                triangle.setVn0idx(int_var[1]);
                triangle.setVn1idx(int_var[3]);
                triangle.setVn2idx(int_var[5]);
                this->triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5) == 6)
            {
                // only texture mapping enabled, use the format v/vt
                // smooth shading not applied
                // texture mapping applied
                Triangle triangle;
                triangle.setID(obj_triangle_idx++);
                triangle.setMidx(m_idx);
                triangle.setTextureidx(texture_idx);
                triangle.setBumpidx(bump_idx);
                triangle.setV0idx(int_var[0]);
                triangle.setV1idx(int_var[2]);
                triangle.setV2idx(int_var[4]);
                triangle.setTextureMap(true);
                triangle.setVt0idx(int_var[1]);
                triangle.setVt1idx(int_var[3]);
                triangle.setVt2idx(int_var[5]);
                this->triangle_list.push_back(triangle);
            }
            else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                            int_var, int_var + 1, int_var + 2,
                            int_var + 3, int_var + 4, int_var + 5,
                            int_var + 6, int_var + 7, int_var + 8) == 9)
            {
                // smooth shading, and texture mapping, use the format v/vt/vn
                Triangle triangle;
                triangle.setID(obj_triangle_idx++);
                triangle.setMidx(m_idx);
                triangle.setTextureidx(texture_idx);
                triangle.setBumpidx(bump_idx);
                triangle.setV0idx(int_var[0]);
                triangle.setV1idx(int_var[3]);
                triangle.setV2idx(int_var[6]);
                triangle.setSmoothShade(true);
                triangle.setVn0idx(int_var[2]);
                triangle.setVn1idx(int_var[5]);
                triangle.setVn2idx(int_var[8]);
                triangle.setTextureMap(true);
                triangle.setVt0idx(int_var[1]);
                triangle.setVt1idx(int_var[4]);
                triangle.setVt2idx(int_var[7]);
                this->triangle_list.push_back(triangle);
            }
        }
    }
    inputstream.close();
    return num_keywords;
}
