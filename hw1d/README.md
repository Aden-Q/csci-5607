# CSCI 5607 Assignment 1d: Transparency and Mirror Reflections

## Description

The program reads scene description from a file (as a command line argument). It runs a recursive ray tracing algorithm to determine the illumination of any intersection point of a ray with objects in the scene. The program is capable of rendering spheres and triangles using both flat shading and smooth shading.  The program also implements texture mapping, for which is is capable of rendering texture spheres and triangles.

It generates an illuminated image in `ppm` format.

## File Organization

+ `src`: all the source code and makefile
+ `input`: scene description input files for testing
+ `output`: output files by the program
+ `image`: images used in the README
+ `test_case`: several cases to test the correctness and performance of the program, each folder contains a result image along with the corresponding input file
+ `showcase`: the folder contains a showcase image along with the corresponding scene description input file

## Implementation Details

1. Create a `Scene` object, read from the scene description file, and construct the scene using `parseScene`.
2. Construct a viewing window using `view_window_init`.
3. For each pixel in the image, run a ray tracing algorithm using `trace_ray`.
3. The `trace_ray` function will recursively call `trace_ray_recursive` to emulate reflection and transmission of rays, at a maximum recursion depth 5.
4. For each ray, checking whether it intersects with any object in the scene, using `intersect_check`.
5. When intersecting, if texture mapping or smooth shading enabled, run them separately to determine the normal direction at each point, and the diffuse color to retrieve.
6. Use the extended Blinn-Phong illumination model and shadowing effects to determine the color for that pixel.
7. Once all pixels in the image are rendered, generate an output image in `ppm` format.

## Extended Blinn-Phong illumination

The Phong illumination model is an empirical model that trys to emulate the appearance of common materials under simple lighting conditions. It does not emulate the physical effects.

$$
I_\lambda = k_aO_{d\lambda} + S \cdot IL_{\lambda}[k_dO_{d\lambda} (\vec{N} \cdot \vec{L}) + k_sO_{s\lambda} (\vec{N} \cdot \vec{H})^n] + \sum F_r^d \cdot R_\lambda + \sum (1-F_r)^d \cdot (e^{-\alpha_\lambda t}) \cdot T_\lambda
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
+ $\sum F_r^d \cdot R_\lambda$: The accumulate contribution from the specularly reflected ray
+ $\sum (1-F_r)^d \cdot (e^{-\alpha_\lambda t}) \cdot T_\lambda$: The accumulate contribution from the specularly transmitted ray

## Usage

+ Enter the `src` foler.
+ Type `clear && make clean && make && ./raytracer ../input/hw1c/rainbow.txt`. The last command argument is the path to the scene description file.
+ It will generate a `ppm` file in the `output` folder as the input file if the input file format is correct.

## Showcase Image

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282157269.jpg)


## Reflected Ray

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282020340.png)

Given the (normalized) incidence ray $\vec{I}$, and the (normalized )surface normal $\vec{N}$. We can calculate the reflection ray as following:
$$
\vec{R} = \vec{A} + \vec{S} = (a\vec{N}) + (a\vec{N} - \vec{I}) = 2(a\vec{N}) - \vec{I} = 2 (\vec{N} \cdot \vec{I}) \vec{N} - \vec{I}
$$
And the reflected ray origins at the intersection point between the incident ray and surface. Once we have the new direction $\vec{R}$, we trace the ray along that direction again until we reach the pre-defined maximum recursion depth (5 in our case).

And the effects of reflection by my program is shown below

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282133137.jpg)

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282134066.jpg)

## Transmitted Ray

We have a parameter $\alpha$ to specify the opacity of objects for each material. For an object that is partially or completely transparent, like a drinking glass filled with water. The incident ray can travel through the object and thus we can see the scene behind the object. We use a simple approach to rendering transparent objects by simply adding the intensity returned by the transmitted ray to the total intensity at a ray/surface intersection point.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282039409.png)

By Snell's Law, we have
$$
\frac{\sin \theta_{i}}{\sin \theta_t} = \frac{\eta_t}{\eta_i}
$$
where $\eta_i$ and $\eta_t$ are the indices of refraction of the materials that the incoming and transmitted rays pass through, respectively.

In the figure above, we can calculate the transmitted ray as:
$$
\vec{T} = \vec{A} + \vec{B} \\
\vec{T} = \cos\theta_t(-\vec{N}) + \frac{\eta_i}{\eta_t}(\cos\theta_i \vec{N} - \vec{I}) \\
\cos\theta_t = \sqrt{1-\sin^2\theta_t} \\
\cos\theta_t = \sqrt{1-[(\eta_i/\eta_t)\sin\theta_i]^2} \\
\cos\theta_t = \sqrt{1-[(\eta_i/\eta_t)^2(1-\cos^2\theta_i)]} \\
\vec{T} = (-\vec{N}) \sqrt{1-[(\eta_i/\eta_t)^2(1-\cos^2\theta_i)]} \frac{\eta_i}{\eta_t}(\cos\theta_i \vec{N} - \vec{I})
$$
We model the absorption of light by using Beer's Law. The intensity $T_\lambda$ returned by a transmitted ray is attenuated by the factor $e^{-\alpha_\lambda t}$, where $\alpha_{\lambda}$ is a user provided and wavelength-dependent parameters. And $t$ is the accumulated distance that the ray travels through the transparent medium. In our model, for simplicity, we assume $\alpha_{\lambda}$  to be consistent across different lights.

And the effects of refraction by my program is shown below

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282136547.jpg)

### Total Internal Reflection

When the incidence ray is traveling through a medium that has a larger index of refraction than the medium on the other side of the intersection point. A phenomenon called "Total Internal Reflection" (TIR) can possibly happen, in which case there is no transmitted ray, illustrated as following:

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203282048804.png)

When we increase the angle of the incidence ray, the angle of the transmitted ray will also increase. In the case when the angle of the transmitted ray is larger than 90 degrees, the transmitted ray will disappear and  only reflection happens. By Snell's Law, we have
$$
\frac{\sin \theta_{i}}{\sin 90^。} = \frac{\eta_t}{\eta_i} \\
\sin \theta_i > \frac{\eta_t}{\eta_i}
$$
If the condition above is satisfied, the angle of refraction $\theta_{t}$ is undefined. In my program, total internal reflection is handled by checking the condition above and stop tracing the transmitted ray if the condition is satisfied.

## Fresnel Reflectance

### Schlick's Approximation

$F_r$ is the Fresnel reflectance coefficient, a fractional weight that depends both on the angle between the incident ray and the surface normal directions, and on the material’s index of refraction. $F_r$ Controls how reflective the surface is at a given point. In 1994, Chirstophe Schlick presented a simple method for efficiently approximating Fresnel reflectance. The equations are as following:
$$
F_r = F_0 + (1-F_0)(1-\cos\theta)^5 \\
F_0 = (\frac{\eta_t-\eta_i}{\eta_t+\eta_i})^2
$$
Where $\eta_i$ is the index of refraction of the material where the incidence ray is in. And $\eta_t$ is the index of refraction of the objects. I use this approximation in my program.

## Shadowing by Transparent Surfaces

Previously, my program only handles situations with opaque objects in the scene so we set a shadow flag to be either 0 or 1. In this assignment, we are going to deal with transparent surfaces so we extend the shadowing flag by attenuating it gradually.

Each time when we need to decide shadowing effects, we cast a secondary ray towards the light and along that direction, we check intersection between the ray and any object. If there is an intersection, then we attenuate the flag $S$ by:
$$
S = S \cdot (1-\alpha_i)
$$
Where $\alpha_i$ is the opacity of each surface that is encountered along the ray.

## Extra Credit

Not attempted

## Known Issues

Not discovered yet

## Credits

Zecheng Qian (qian0102@umn.edu)
