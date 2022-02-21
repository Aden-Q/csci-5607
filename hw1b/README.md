# CSCI 5607 Assignment 1b

## Description

The program reads scene description from a file (as a command line argument). It runs a ray tracing algorithm to determine the intersection point between the ray and objects in the scene. Then it use the Blinn-Phong illumination model to define the color for that point. The program also implements shadows using backward ray tracing.

It generates an illuminated image in `ppm` format.

## File Organization

+ `src`: all the source code and makefile
+ `input`: scene description input file for testing
+ `image`: images used in the README, comparing different parameter settings
+ `test_case`: several cases to test the correctness and performance of the program, each folder contains a result image along with the corresponding input file
+ `showcase`: the folder contains a showcase image along with the corresponding scene description input file

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
+ $S$: a flag to determine the shadowing effect
+ $IL_{\lambda}$: intensity of a light source

## Usage

+ Enter the `src` foler.
+ Type `clear && make clean && make && ./raytracer ../input/hw1b/ray_input1b.txt`. The last command argument is the path to the scene description file.
+ It will generate a `ppm` file in the same folder as the input file if the input file format is correct.

## Showcase Image

## Effects of illumination model Parameters

### Effects of $O_{dl}$

From left to right, use $O_{dl}=(1,0,0), (0,1,0), (0,0,1)$ (Red, Green, and Blue) separately. In all images, $O_{sl} = (1,1,1)$, $k_a = 0.1$, $k_d = 0.5$, $k_s = 0.3$, $n = 20$.

Because $O_{dl}$ defines the intrinsic color of the objects. The visual appearance changes when this parameter varies.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201625536.jpg)

### Effects of $O_{sl}$

From left to right, use $O_{sl}=(1,1,1), (1,1,0), (0,0,1)$ (White, Yellow, and Blue) separately. In all images, $O_{dl} = (1,0,0)$, $k_a = 0.1$, $k_d = 0.5$, $k_s = 0.3$, $n = 20$.

The parameter $O_{sl}$ specifies the specular color of the material. We can see that in the central area of the ball where light reflecetion is the most intensive, its apprearance is controlled by this parameter.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201637072.jpg)

### Effects of $k_a$

From left to right, use $k_a=0.1, 0.5, 0.8$ separately. In all images, $O_{dl}=(1,0,0)$, $ O_{sl} = (1,1,1)$, $k_d = 0.5$, $k_s = 0.3$, $n = 20$.

This parameter specifies to what extent the intrinsic color (in this case Red) dominates the final color of a pixel, and uniformly reflects light that arrives at it indirectly. As we increase this parameter, we can see that the object's color is more close to pure Red and more distorted.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201644663.jpg)

### Effects of $k_d$

From left to right, use $k_d=0.1, 0.5, 0.8$ separately. In all images, $O_{dl}=(1,0,0)$, $ O_{sl} = (1,1,1)$, $k_a = 0.1$, $k_s = 0.3$, $n = 20$.

This parameter is the weight of the diffuse term, it specifies to what extent the object reflect directly incoming light in a diffuse manner. As we increase this parameter, we can see that the visual appearance becomes brighter and brighter.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201652701.jpg)

### Effects of $k_s$

From left to right, use $k_s=0.1, 0.3, 0.8$ separately. In all images, $O_{dl}=(1,0,0)$, $ O_{sl} = (1,1,1)$, $k_a = 0.1$, $k_d = 0.5$, $n = 20$.

This parameter is the weight of the specular term, it specifies to what extent the object reflect directly incoming light in a specular manner.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201657232.jpg)

### Effects of $n$

From left to right, use $n=2, 20, 50$ separately. In all images, $O_{dl}=(1,0,0)$, $ O_{sl} = (1,1,1)$, $k_a = 0.1$, $k_d = 0.5$, $k_s = 0.3$.

This parameter also controls the specular term, it specifies to what extent the object reflect directly incoming light in a specular manner. As we can see from the image below, when $n$ is very small, the resulting image is distorted and the illumination model cannot simulate the visual appearance of objects very well.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201700019.jpg)

### Point Light Source VS. Directional Light Source

We use the same settings. The only difference is that: the left image is produced by using a point light source and the right one is used by using a directional light source. We can see that reflection effects of light on the surface of the object can be better captured by using a point light source.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202201707023.jpg)

### Use of Multiple Light Source

The image below shows the effects of using multiple light sources, including two point light sources and one directional light source. We can obviously see the effects of reflection on objects surface corresponding to different light source.

![img](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202202202256927.jpg)

### Conclusion

In conclusion, we can see that the illumination model does not always work well. In some settings such as very small $n$, very large $k_s$, very small $k_d$, very large $k_a$, the resulting images are more likely distorted.

## Extra Credit

### Soft Shadows

### Light Source Attenuation

In real cases, the energy of a light will attenuate when the light's travel distance increases. As a result of which the intensity of the illumination (from a point light source) will also fall off with distance. We use the following formula to emulate the effect of light source attenuation:

### Depth Cueing

## Known Issues

When I am implementing the program, I find that there might be some issue when there is some object in scene which is right in front of the viewer with a distance almost equal to 0. This basically means that the viewer cannot see anything in the scene because its eye is obstructed by the object in front of him/her. Although this is not realistic, it is an edge case that worth considering.

Besides, I found that the generated image is not very smooth. This might be due to the resolution and size of the final image. Usually our program generates small size images. But this non-smooth effect can be optimized using some strategies.

## Credits

Zecheng Qian (qian0102@umn.edu)
