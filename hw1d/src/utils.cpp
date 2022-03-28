/**
 * @file utils.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
#include "utils.h"
#include "types.h"

void output_image(std::string filename, Color **checkerboard, int width, int height)
{
    std::ofstream outputstream(filename, std::ios::out);

    if (!outputstream.is_open())
    {
        fprintf(stderr, "Could not open output stream with file %s\n", filename.c_str());
    }

    uint8_t pixel_counter = 0;
    // create a ppm image in P3 format
    outputstream
        << "P3" << std::endl
        << width << " "
        << height << std::endl
        << MAX_VAL << std::endl;
    // fill in color for each pixel
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            outputstream << (int)(checkerboard[x][y].getR() * MAX_VAL) << " "
                         << (int)(checkerboard[x][y].getG() * MAX_VAL) << " "
                         << (int)(checkerboard[x][y].getB() * MAX_VAL) << " ";
            pixel_counter++;
            // start a new line if 4 pixels are filled
            if (pixel_counter % 4 == 0)
            {
                pixel_counter = 0;
                outputstream << std::endl;
            }
        }
    }

    outputstream.close();
    return;
}

float distance_between_2D_points(FloatVec2 point1, FloatVec2 point2)
{
    float sum = pow(point1.first - point2.first, 2) 
            + pow(point1.second - point2.second, 2);

    return sqrt(sum);
}

float distance_between_3D_points(FloatVec3 point1, FloatVec3 point2)
{
    float sum = pow(point1.first - point2.first, 2) 
            + pow(point1.second - point2.second, 2)
            + pow(point1.third - point2.third, 2);
    
    return sqrt(sum);
}

bool lie_within(float p, float e1, float e2)
{
    if (p > e1 && p < e2) 
    {
        return true;
    }
    else if (p > e2 && p < e1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void view_window_init(const Scene &scene, ViewWindow &viewwindow, float viewdist) 
{
    // directional vectors
    FloatVec3 viewdir = scene.getViewdir();
    FloatVec3 updir = scene.getUpdir();
    int width = scene.getWidth();
    int height = scene.getHeight();
    float vfov = scene.getVfov();
    FloatVec3 eye = scene.getEye();
    viewwindow.viewdist = viewdist;
    viewwindow.u = viewdir.cross(updir).normal();
    viewwindow.v = viewwindow.u.cross(viewdir).normal();
    // size of the view window
    viewwindow.height = 2 * viewwindow.viewdist * tan(vfov / 2 * PI / 180.0);
    // use aspect ratio match to determine width
    viewwindow.width = viewwindow.height * width * 1.0 / height;
    // calculate four corner points
    FloatVec3 n = viewdir.normal();
    viewwindow.ul = eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ur = eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) + viewwindow.v * (viewwindow.height / 2);
    viewwindow.ll = eye + n * viewwindow.viewdist - viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    viewwindow.lr = eye + n * viewwindow.viewdist + viewwindow.u * (viewwindow.width / 2) - viewwindow.v * (viewwindow.height / 2);
    // calculate offset vector in the current viewing window (per pixel offset)
    viewwindow.dh = (viewwindow.ur - viewwindow.ul) / (width - 1);
    viewwindow.dv = (viewwindow.ll - viewwindow.ul) / (height - 1);   
}

const MaterialColor &get_material(const Scene &scene, std::string obj_type, int obj_idx)
{
    
    if (obj_type == "Sphere")
    {
        return scene.getMaterialList()[scene.getSphereList()[obj_idx].getMidx()];
    }
    else if (obj_type == "Triangle")
    {
        return scene.getMaterialList()[scene.getTriangleList()[obj_idx].getMidx()];
    }
    // placeholder for other types of objects
    else
    {
        return scene.getMaterialList()[scene.getTriangleList()[obj_idx].getMidx()];
    }
}

FloatVec3 get_normal(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p)
{
    if (obj_type == "Sphere")
    {
        return scene.getSphereList()[obj_idx].normal(p);
    }
    else if (obj_type == "Triangle")
    {
        return scene.getTriangleList()[obj_idx].normal(scene ,p);
    }
    // placeholder for other types of objects
    else
    {
        return scene.getTriangleList()[obj_idx].normal(scene, p);
    }
}

bool texture_map_enabled(const Scene &scene, std::string obj_type, int obj_idx)
{
    if (obj_type == "Sphere")
    {
        return (scene.getSphereList()[obj_idx].getTextureidx() != -1);
    }
    else if (obj_type == "Triangle")
    {
        return scene.getTriangleList()[obj_idx].getTextureMap();
    }
    // placeholder for other types of objects
    else
    {
        return true;
    }
}

bool normal_map_enabled(const Scene &scene, std::string obj_type, int obj_idx)
{
    if (obj_type == "Sphere")
    {
        return scene.getSphereList()[obj_idx].getBumpidx() != -1;
    }
    else if (obj_type == "Triangle")
    {
        return (scene.getTriangleList()[obj_idx].getBumpidx() != -1);
    }
    // placeholder for other types of objects
    else
    {
        return true;
    }
}

const Texture &get_texture(const Scene &scene, std::string obj_type, int obj_idx)
{
    if (obj_type == "Sphere")
    {
        const Texture &texture = scene.getTextureList()[scene.getSphereList()[obj_idx].getTextureidx()];
        return texture;
    }
    else if (obj_type == "Triangle")
    {
        const Texture &texture = scene.getTextureList()[scene.getTriangleList()[obj_idx].getTextureidx()];
        return texture;
    }
    // placeholder for other types of objects
    else
    {
        const Texture &texture = scene.getTextureList()[scene.getTriangleList()[obj_idx].getTextureidx()];
        return texture;
    }
}

const Bump &get_normal_map(const Scene &scene, std::string obj_type, int obj_idx)
{
    if (obj_type == "Sphere")
    {
        const Bump &bump = scene.getBumpList()[scene.getSphereList()[obj_idx].getBumpidx()];
        return bump;
    }
    else if (obj_type == "Triangle")
    {
        const Bump &bump = scene.getBumpList()[scene.getTriangleList()[obj_idx].getBumpidx()];
        return bump;
    }
    // placeholder for other types of objects
    else
    {
        const Bump &bump = scene.getBumpList()[scene.getTriangleList()[obj_idx].getBumpidx()];
        return bump;
    }
}

FloatVec2 get_texture_coordinate(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p)
{
    FloatVec2 texture_cor;
    if (obj_type == "Sphere")
    {
        texture_cor = scene.getSphereList()[obj_idx].texture_coordinate(p);
    }
    else if (obj_type == "Triangle")
    {
        texture_cor = scene.getTriangleList()[obj_idx].texture_coordinate(scene, p);
    }
    // placeholder for other types of objects
    else
    {
        texture_cor = scene.getTriangleList()[obj_idx].texture_coordinate(scene, p);
    }

    return texture_cor;
}

Color get_color(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p)
{
    FloatVec2 texture_cor = get_texture_coordinate(scene, obj_type, obj_idx, p);
    const Texture &texture = get_texture(scene, obj_type, obj_idx);
    int width = texture.getWidth();
    int height = texture.getHeight();
    Color **checkerboard = texture.getCheckerboard();
    float u = texture_cor.first;
    float v = texture_cor.second;
    // bi-linear interpolation to get the color from the texture image
    float x = u * (width - 1);
    float y = v * (height - 1);
    int i = int(x);
    int j = int(y);
    float alpha = x - i;
    float beta = y - j;
    Color pixel0 = checkerboard[i][j];
    Color pixel1 = checkerboard[i+1][j];
    Color pixel2 = checkerboard[i][j+1];
    Color pixel3 = checkerboard[i+1][j+1];

    return Color(pixel0 * (1 - alpha) * (1 - beta) +
                 pixel1 * alpha * (1 - beta) +
                 pixel2 * (1 - alpha) * beta +
                 pixel3 * alpha * beta);
}

FloatVec3 normal_mapping(const Scene &scene, std::string obj_type, int obj_idx, FloatVec3 &p)
{
    // first get the texture coordinate of the object
    FloatVec2 texture_cor = get_texture_coordinate(scene, obj_type, obj_idx, p);
    const Texture &texture = get_texture(scene, obj_type, obj_idx);
    const Bump &bump = get_normal_map(scene, obj_type, obj_idx);
    // get the surface normal
    FloatVec3 N = get_normal(scene, obj_type, obj_idx, p);
    int width = texture.getWidth();
    int height = texture.getHeight();
    // pixel coordinate
    float u = texture_cor.first;
    float v = texture_cor.second;
    float x = u * (width - 1);
    float y = v * (height - 1);
    // convert to pixel coordinate in the normal map
    // use nearest neighboor for simplicity
    int i = std::round(x);
    int j = std::round(y);
    // retrieve the normal direction from the normal map
    FloatVec3 m = bump.getNormal(i, j);
    // calculate the modified normal
    // consider differently for spheres and triangles
    if (obj_type == "Sphere")
    {
        FloatVec3 N = get_normal(scene, obj_type, obj_idx, p);
        FloatVec3 T(-N.second / sqrt(N.first * N.first + N.second * N.second),
                    N.first / sqrt(N.first * N.first + N.second * N.second),
                    0);
        FloatVec3 B = N.cross(T);
        // use the TBN transformation to transform m
        float nx = T.first * m.first + B.first * m.second + N.first * m.third;
        float ny = T.second * m.first + B.second * m.second + N.second * m.third;
        float nz = T.third * m.first + B.third * m.second + N.third * m.third;
        return FloatVec3(nx, ny, nz);
    }
    else if (obj_type == "Triangle")
    {
        // get three vertices of the triangle
        const Triangle& triangle = scene.getTriangleList()[obj_idx];
        FloatVec3 p0 = scene.getVertexList()[triangle.getV0idx()- 1].p;
        FloatVec3 p1 = scene.getVertexList()[triangle.getV1idx() - 1].p;
        FloatVec3 p2 = scene.getVertexList()[triangle.getV2idx() - 1].p;
        FloatVec2 texture_cor0 = scene.getTextureCoordinateList()[triangle.getVt0idx() - 1].vt;
        FloatVec2 texture_cor1 = scene.getTextureCoordinateList()[triangle.getVt1idx() - 1].vt;
        FloatVec2 texture_cor2 = scene.getTextureCoordinateList()[triangle.getVt2idx() - 1].vt;
        float delta_u1 = texture_cor1.first - texture_cor0.first;
        float delta_v1 = texture_cor1.second - texture_cor0.second;
        float delta_u2 = texture_cor2.first - texture_cor1.first;
        float delta_v2 = texture_cor2.second - texture_cor1.second;
        FloatVec3 e1 = p1 - p0;
        FloatVec3 e2 = p2 - p1;
        // calculate matrices T and B
        FloatVec3 T = (e1 * delta_v2 - e2 * delta_v1) / (1 / (delta_u1 * delta_v2 - delta_v1 * delta_u2));
        FloatVec3 B = (e1 * (-delta_u2) + e2 * delta_u1) / (1 / (delta_u1 * delta_v2 - delta_v1 * delta_u2));
        // use the TBN transformation to transform m
        float nx = T.first * m.first + B.first * m.second + N.first * m.third;
        float ny = T.second * m.first + B.second * m.second + N.second * m.third;
        float nz = T.third * m.first + B.third * m.second + N.third * m.third;
        return FloatVec3(nx, ny, nz);
    }
    // placeholder for other types of objects
    else
    {
        return get_normal(scene, obj_type, obj_idx, p);
    }
}

Color shade_ray(const Scene &scene, std::string obj_type, int obj_idx, const Ray &ray, float ray_t)
{
    // use The Phong Illumination Model to determine the color of the intersecting point
    // return the corresponding color for that object
    // the surface normal should be consider differently for sphere and cylinder
    // compute the intersection point
    FloatVec3 p = ray.extend(ray_t);
    const MaterialColor &cur_material = get_material(scene, obj_type, obj_idx);
    Color Od_lambda = cur_material.getOd();
    // compute the coresponding color from the texture coordinate
    if (texture_map_enabled(scene, obj_type, obj_idx))
    {
        Od_lambda = get_color(scene, obj_type, obj_idx, p);
    }
    float Ir, Ig, Ib;
    float sum_r = cur_material.getKa() * Od_lambda.getR();
    float sum_g = cur_material.getKa() * Od_lambda.getG();
    float sum_b = cur_material.getKa() * Od_lambda.getB();
    Color res_color;
    float f_att = 1; // light source attenuation factor
    const std::vector<Light> &light_list = scene.getLightList();
    const std::vector<AttLight> &attlight_list = scene.getAttLightList();
    // light intensity for each component, just take the average for simplicity
    float IL = 1.0 / (light_list.size() + attlight_list.size());
    // illumination for normal light
    for (int i = 0; i < light_list.size(); i++)
    {
        Light light = light_list[i];
        res_color = light_shade(scene, ray, ray_t, light, obj_type, obj_idx);
        Ir = res_color.getR();
        Ig = res_color.getG();
        Ib = res_color.getB();

        sum_r += IL * Ir;
        sum_g += IL * Ig;
        sum_b += IL * Ib;
    }

    // illumination for attenuated light
    for (int i = 0; i < attlight_list.size(); i++)
    {
        AttLight attlight = attlight_list[i];
        res_color = light_shade(scene, ray, ray_t, attlight, obj_type, obj_idx);
        Ir = res_color.getR();
        Ig = res_color.getG();
        Ib = res_color.getB();
        if ((attlight.w - 1) < 1e-6)
        {
            f_att = light_attenuation(p, attlight);
        }

        sum_r += f_att * IL * Ir;
        sum_g += f_att * IL * Ig;
        sum_b += f_att * IL * Ib;
    }

    // clamping
    sum_r = std::min(float(1.0), sum_r);
    sum_g = std::min(float(1.0), sum_g);
    sum_b = std::min(float(1.0), sum_b);

    // apply depth cueing if enabled
    if (scene.depthCueEnable())
    {
        // depth cueing factor
        float depth_cue_coeff = depth_cueing(p, scene.getEye(), scene.getDepthCue());
        DepthCue depth_cue = scene.getDepthCue();
        sum_r = depth_cue_coeff * sum_r + (1 - depth_cue_coeff) * depth_cue.dc_r;
        sum_g = depth_cue_coeff * sum_g + (1 - depth_cue_coeff) * depth_cue.dc_g;
        sum_b = depth_cue_coeff * sum_b + (1 - depth_cue_coeff) * depth_cue.dc_b;
    }

    return Color(sum_r, sum_g, sum_b);
}

Color light_shade(const Scene &scene, const Ray &ray, float ray_t, const Light &light, std::string obj_type, int obj_idx)
{
    // get the intersection point
    FloatVec3 p = ray.extend(ray_t);
    FloatVec3 dir = ray.getDir();
    const MaterialColor &cur_material = get_material(scene, obj_type, obj_idx);
    Color Od_lambda = cur_material.getOd();
    Color Os_lambda = cur_material.getOs();
    FloatVec3 N = get_normal(scene, obj_type, obj_idx, p);
    // calculate vector L
    FloatVec3 L;
    // compute the coresponding color from the texture coordinate
    if (texture_map_enabled(scene, obj_type, obj_idx))
    {
        Od_lambda = get_color(scene, obj_type, obj_idx, p);
        if (normal_map_enabled(scene, obj_type, obj_idx))
        {
            // if the normal map option is enabled, modified the current surface normal
            N = normal_mapping(scene, obj_type, obj_idx, p);
        }
    }
    if (std::abs(light.w - 1) < 1e-6) // point light source
    {
        L = FloatVec3(light.x - p.first, light.y - p.second, light.z - p.third).normal();
    }
    else if (std::abs(light.w - 0) < 1e-6) // directional light source
    {
        L = FloatVec3(-light.x, -light.y, -light.z).normal();
    }
    // calculate vector H
    // get the vector v
    FloatVec3 V = FloatVec3(-dir.first, -dir.second, -dir.third).normal();
    FloatVec3 H;
    H = (V + L).normal();

    // apply the phong illumination model
    float Ir, Ig, Ib;    // illumination components for R, G and B
    int shadow_flag = 1; // shadowing effect flag, 1 when not in shadow, 0 when in shadow

    // check for shadowing effect
    // cast a second ray forwarding from the intersection point
    // to the point light source,
    // and check for intersection with objects in the scene
    Ray ray_second(p, L);
    // check intersection, if intersected, set the flag to be 0
    if (shadow_check(scene, ray_second, light))
    {
        shadow_flag = 0;
    }

    float term1 = std::max(float(0), N.dot(L));
    float term2 = pow(std::max(float(0), N.dot(H)), cur_material.getN());
    Ir = shadow_flag * (cur_material.getKd() * Od_lambda.getR() * term1 + cur_material.getKs() * Os_lambda.getR() * term2);
    Ig = shadow_flag * (cur_material.getKd() * Od_lambda.getG() * term1 + cur_material.getKs() * Os_lambda.getG() * term2);
    Ib = shadow_flag * (cur_material.getKd() * Od_lambda.getB() * term1 + cur_material.getKs() * Os_lambda.getB() * term2);

    return Color(Ir, Ig, Ib);
}

float light_attenuation(const FloatVec3 &point, const AttLight &light)
{
    // light source attentuation parameters
    float c1 = light.c1;
    float c2 = light.c2;
    float c3 = light.c3;
    float f_att = 1; // attenuation factor
    float dist;      // distance

    if (std::abs(light.w - 1.0) < 1e-6)
    {
        dist = distance_between_3D_points(point, FloatVec3(light.x, light.y, light.z));
        f_att = 1.0 / (c1 + c2 * dist + c3 * dist * dist);
        // clamping
        f_att = std::min(float(1.0), f_att);
    }
    else
    {
        f_att = 1;
    }

    return f_att;
}

float depth_cueing(const FloatVec3 &point, const FloatVec3 &viewer, const DepthCue &depth_cue)
{
    float alpha_max = depth_cue.alpha_max;
    float alpha_min = depth_cue.alpha_min;
    float d_near = depth_cue.dist_min;
    float d_far = depth_cue.dist_max;
    float alpha = 1.0;
    float d_obj = distance_between_3D_points(point, viewer);
    // consider three difference cases: whether the object is near, far from the viewer
    // apply different formula according to the distance between the two
    if (d_obj < d_near)
    {
        alpha = alpha_max;
    }
    else if (d_obj < d_far)
    {
        alpha = alpha_min + (alpha_max - alpha_min) * (d_far - d_obj) / (d_far - d_near);
    }
    else
    {
        alpha = alpha_min;
    }

    return alpha;
}

std::tuple<std::string, int, float> intersect_check(const Scene &scene, const Ray &ray)
{
    float min_t = 100000;
    float temp_t;
    int obj_idx = -1;              // the ID (index) of the intersected object
    std::string obj_type = "None"; // the type of the intersected object with the ray
    float A, B, C, D;
    float ray_t;
    float determinant;
    FloatVec3 p; // intersection point
    FloatVec3 ray_center, obj_center;
    FloatVec3 dir;

    // check intersection for spheres
    for (auto s : scene.getSphereList())
    {
        ray_center = ray.getCenter();
        obj_center = s.getCenter();
        dir = ray.getDir();
        B = 2 * (dir.first * (ray_center.first - obj_center.first) +
                 dir.second * (ray_center.second - obj_center.second) +
                 dir.third * (ray_center.third - obj_center.third));
        C = pow(ray_center.first - obj_center.first, 2) +
            pow(ray_center.second - obj_center.second, 2) +
            pow(ray_center.third - obj_center.third, 2) -
            pow(s.getRadius(), 2);
        determinant = pow(B, 2) - 4 * C;
        if (determinant > -1e-6) // greater than or equal to 0
        {                        // need further check
            temp_t = (-B - sqrt(determinant)) / 2;
            if (temp_t > 1e-3 && temp_t < min_t)
            {
                min_t = temp_t;
                obj_idx = s.getID();
                obj_type = "Sphere";
            }
            // check for another possible solution
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 1e-3 && temp_t < min_t)
            {
                min_t = temp_t;
                obj_idx = s.getID();
                obj_type = "Sphere";
            }
        }
    }

    // check intersection for triangles
    for (auto t : scene.getTriangleList())
    {
        ray_center = ray.getCenter();
        dir = ray.getDir();
        // parameters for the plane equation Ax + By + Cz + D = 0
        FloatVec3 p0 = scene.getVertexList()[t.getV0idx() - 1].p;
        FloatVec3 p1 = scene.getVertexList()[t.getV1idx() - 1].p;
        FloatVec3 p2 = scene.getVertexList()[t.getV2idx() - 1].p;
        FloatVec3 e1 = p1 - p0;
        FloatVec3 e2 = p2 - p0;
        FloatVec3 n = e1.cross(e2).normal();
        A = n.first;
        B = n.second;
        C = n.third;
        D = -A * p0.first - B * p0.second - C * p0.third;
        determinant = A * dir.first + B * dir.second + C * dir.third;
        // test whether the ray is parallel with the plane
        if (std::abs(determinant) < 1e-6)
        {
            // in the case that the ray is parallel to the plane
            continue;
        }
        ray_t = -(A * ray_center.first + B * ray_center.second + C * ray_center.third + D) / determinant;
        if (ray_t < 0)
        {
            // no intersection
            continue;
        }
        // get the intersection point p
        p = ray.extend(ray_t);
        FloatVec3 bayrcentric_coordinates = t.barycentric(scene, p);
        float alpha = bayrcentric_coordinates.first;
        float beta = bayrcentric_coordinates.second;
        float gamma = bayrcentric_coordinates.third;
        // test whether the intersection point is inside the triangle or not
        if (alpha > -1e-6 && alpha < 1 && beta > -1e-6 && beta < 1 && gamma > -1e-6 && gamma < 1)
        {
            // in the triangle
            if (ray_t < min_t)
            {
                min_t = ray_t;
                obj_idx = t.getID();
                obj_type = "Triangle";
            }
        }
        else
        {
            // if not inside the triangle, do nothing
            continue;
        }
    }

    return std::make_tuple(obj_type, obj_idx, min_t);
}

bool shadow_check(const Scene &scene, const Ray &ray, const Light &light)
{
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray);

    if (obj_type != "None")
    {
        // if it is a point light source
        // need further check whether the object is within the range between
        // the starting point of the ray and the point light source
        if ((light.w - 1) < 1e-6)
        {
            float max_t = (light.x - ray.getCenter().first) / ray.getDir().first;
            if (ray_t > 1e-6 && ray_t < max_t)
            {
                return true;
            }
        }
        else
        {
            // in the case of directional light source,
            // as long as the returned t is positive, there is shadow
            if (ray_t > 1e-6)
            {
                return true;
            }
        }
    }

    return false;
}

Color trace_ray_reflective(const Scene &scene, const Ray &ray, int depth, std::string exclude_type, int exclude_id)
{
    // termination condition
    if (depth > MAX_DEPTH)
    {
        return Color(0, 0, 0);
    }
    // compute the reflection ray equation and the Fresnel reflectance coefficient
    FloatVec3 p = ray.getCenter();
    FloatVec3 N = get_normal(scene, exclude_type, exclude_id, p);
    const FloatVec3 &ray_dir = ray.getDir().normal();
    const MaterialColor &mtl = get_material(scene, exclude_type, exclude_id);
    float eta_i = 1.0; // incident from air
    float eta_t = mtl.getEta();
    float F_0 = pow((eta_t - eta_i) / (eta_t + eta_i), 2);
    F_0 = 0.2;
    float F_r = F_0 + (1 - F_0) * pow(1 - N.dot(ray_dir), 5);
    // compute the reflective ray
    FloatVec3 R = (N * 2 * N.dot(ray_dir) - ray_dir).normal();
    Ray ray_reflected(p, R);
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // initialize the response color to be the background color
    Color res_color(0, 0, 0);
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray_reflected);
    if (obj_type != "None")
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray_reflected, ray_t);
    } else
    {
        return res_color;
    }
    // new intersection point
    // new normal vector N, at the new intersection point
    FloatVec3 new_p = ray_reflected.extend(ray_t);
    // new incident ray
    FloatVec3 new_dir = -ray_reflected.getDir().normal();
    Ray new_ray_incident(new_p, new_dir);
    return res_color * pow(F_r, depth) + trace_ray_reflective(scene, new_ray_incident, depth + 1, obj_type, obj_idx);
}

Color trace_ray_transmitive(const Scene &scene, const Ray &ray, int depth, bool flag_enter, float eta_i, std::string exclude_type, int exclude_id)
{
    // termination condition
    if (depth > MAX_DEPTH)
    {
        return Color(0, 0, 0);
    }
    // compute the transmitive ray
    const MaterialColor &mtl = get_material(scene, exclude_type, exclude_id);
    float eta_t = mtl.getEta();
    FloatVec3 p = ray.getCenter();
    FloatVec3 N = get_normal(scene, exclude_type, exclude_id, p);
    const FloatVec3 &ray_dir = ray.getDir().normal();
    if (!flag_enter)
    {
        eta_t = 1.0;  // air if exiting
        N = -N;
    }
    // compute the Fresnel reflectance coefficient
    float F_0 = pow((eta_t - eta_i) / (eta_t + eta_i), 2);
    F_0 = 0.2;
    float F_r = F_0 + (1 - F_0) * pow(1 - N.dot(ray_dir), 5);
    // compute the tranmitted ray
    FloatVec3 T = -N * sqrt(1 - pow(eta_i / eta_t, 2) * (1 - pow(N.dot(ray_dir), 2))) + (N * N.dot(ray_dir) - ray_dir) * (eta_i / eta_t);
    Ray ray_tranmitted(p, T);
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    Color res_color(0, 0, 0);
    // loop for all objects
    // check whether there is an intersection
    if (flag_enter)
    {
        std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray_tranmitted);
    } else
    {
        std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray_tranmitted);
    }
    if (obj_type != "None")
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray_tranmitted, ray_t);
    }
    else
    {
        return res_color;
    }
    // new intersection point
    // new normal vector N, at the new intersection point
    FloatVec3 new_p = ray_tranmitted.extend(ray_t);
    // new incident ray
    FloatVec3 new_dir = -ray_tranmitted.getDir().normal();
    Ray new_ray_incident(new_p, new_dir);
    // test total internal reflection
    if (pow(N.dot(ray_dir), 2) < 1 - pow(eta_t / eta_i, 2))
    {
        return res_color * pow(1 - F_r, depth);
    } else
    {
        return res_color * pow(1 - F_r, depth) + trace_ray_transmitive(scene, new_ray_incident, depth + 1, !flag_enter, eta_t, obj_type, obj_idx);
    }
}

Color trace_ray(const Scene &scene, const ViewWindow &viewwindow, int w, int h)
{
    FloatVec3 eye = scene.getEye();
    // get a ray representation
    FloatVec3 point_in_view(viewwindow.ul + viewwindow.dh * w + viewwindow.dv * h);
    FloatVec3 raydir = (point_in_view - eye).normal();
    Ray ray(eye, raydir);  // the first ray
    std::string obj_type;
    int obj_idx;
    float ray_t; // material index
    // initialize the response color to be the background color
    Color res_color(scene.getBkgcolor());
    // loop for all objects
    // check whether there is an intersection
    std::tie(obj_type, obj_idx, ray_t) = intersect_check(scene, ray);
    if (obj_type == "None")
    {
        // if the first ray does not intersect with anything, return the background color
        return res_color;
    } else
    {
        res_color = shade_ray(scene, obj_type, obj_idx, ray, ray_t);
    }
    // normal vector
    const MaterialColor &mtl = get_material(scene, obj_type, obj_idx);
    FloatVec3 p = ray.extend(ray_t);
    const FloatVec3 &ray_dir = ray.getDir();
    // the incident ray
    FloatVec3 I = -ray_dir.normal();
    Ray ray_incidence(p, I);
    Color final_color = res_color + trace_ray_reflective(scene, ray_incidence, 1, obj_type, obj_idx); // exclude the intersected object
    // Color final_color = res_color + trace_ray_recursive(scene, ray_incidence, 1, true, obj_type, obj_idx); // exclude the intersected object

    // if (std::abs(mtl.getAlpha() - 1.0) > 1e-6)
    // {
    //     // transparent surface
    //     final_color = final_color + trace_ray_transmitive(scene, ray_incidence, 1, true, 1.0, obj_type, obj_idx);
    // }

    // clapping
    if (final_color.getR() > 1.0)
    {
        final_color.setR(1.0);
    }
    if (final_color.getG() > 1.0)
    {
        final_color.setG(1.0);
    }
    if (final_color.getB() > 1.0)
    {
        final_color.setB(1.0);
    }

    return final_color;
}