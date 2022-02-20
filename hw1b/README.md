# CSCI 5607 Assignment 1b

## Description

The program reads scene description from a file (as a command line argument). It runs a ray tracing algorithm to determine the intersection point between the ray and objects in the scene. Then it use the Blinn-Phong illumination model to define the color for that point. The program also implements shadows using backward ray tracing.

It generates an illuminated image in `ppm` format.

## File Organization

+ `src`: contains all the source code and makefile.
+ `input`: scene description input file for testing.
+ `output`: some examples the output produced by the program.

## Implementation Details

1. Construct the scene using `parse_scene`.
2. Construct a viewing window using `view_window_init`.
3. For each pixel in the image, run a ray tracing algorithm using `trace_ray`.
4. For each ray, checking whether it intersects with any object in the scene, using `intersect_check`.
5. When intersecting, use the Blinn-Phong illumination model and shadowing effects to determine the color for that point.
6. Once all pixels in the image are rendered, generate an output image in `ppm` format.

## Blinn-Phong illumination

The Phong illumination model is an empirical model that trys to emulate the appearance of common materials under simple lighting conditions. It does not emulate the physical effects.

$$
I_\lambda = k_aO_{d\lambda} + S \cdot IL_{\lambda}[k_dO_{d\lambda} (\vec{N} \cdot \vec{L}) + k_sO_{s\lambda} (\vec{N} \cdot \vec{H})^n]




$$

Components:

+ $k_aO_{d\lambda}$: ambient term which approximates the reflection of light that arrives indirectly
+ $k_dO_{d\lambda}(\vec{N} \cdot \vec{L})$: diffuse term which represents the uniform reflection of light from a light source
+ $k_sO_{s\lambda}(\vec{N} \cdot \vec{H})^n$: specular term which represents the mirror-like reflection of incoming light

Parameter specification:

+ $I_\lambda$ : color intensity
+ $O_{d\lambda}$: intrinsic color of the object
+ $O_{s\lambda}$: specular color of the object, usually the background color
+ $k_a$, $k_d$, $k_s$: weights that control the relative effects of surface's ambient, diffuse, and specular
+ $\vec{N}$: surface normal
+ $\vec{L}$: vector point from the intersecting point towards the light source
+ $\vec{H}$: a derived vector, using the formula $\vec{H} = \frac{\vec{L}+\vec{V}}{||\vec{L}+\vec{V}||}$ , in which $\vec{V}$ is the vector pointing from the intersecting point towards the eye

## Usage

+ Enter the `src` foler.
+ Type `clear && make clean && make && ./raytracer ../input/hw1b/ray_input1b.txt`. The last command argument is the path to the scene description file.
+ It will generate a `ppm` file in the same folder as the input file if the input file format is correct.

## Effects of illumination model Parameters

### Effects of $O_{dl}$



### Effects of $O_{sl}$



### Effects of $k_a$



### Effects of $k_d$



### Effects of $k_s$



### Effects of $n$






## Extra Credit




## Known Issues

When I am implementing the program, I find that there might be some issue when there is some object in scene which is right in front of the viewer with a distance almost equal to 0. This basically means that the viewer cannot see anything in the scene because its eye is obstructed by the object in front of him/her. Although this is not realistic, it is an edge case that worth considering.

Besides, I found that the generated image is not very smooth. This might be due to the resolution and size of the final image. Usually our program generates small size images. But this non-smooth effect can be optimized using some strategies.

## Credits

Zecheng Qian (qian0102@umn.edu)
