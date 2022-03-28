# CSCI 5607 Assignment 1d: Transparency and Mirror Reflections

## Description

The program reads scene description from a file (as a command line argument). It runs a ray tracing algorithm to determine the intersection point of a ray with objects in the scene. The program is capable of rendering triangles using both flat shading and smooth shading.  The program also implements texture mapping, for which is is capable of rendering texture spheres and triangles.

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
4. For each ray, checking whether it intersects with any object in the scene, using `intersect_check`.
5. When intersecting, if texture mapping or smooth shading enabled, run them separately to determine the normal direction at each point, and the diffuse color to retrieve.
6. Use the Blinn-Phong illumination model and shadowing effects to determine the color for that pixel.
7. Once all pixels in the image are rendered, generate an output image in `ppm` format.

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
+ Type `clear && make clean && make && ./raytracer ../input/hw1c/ray_input1b.txt`. The last command argument is the path to the scene description file.
+ It will generate a `ppm` file in the `output` folder as the input file if the input file format is correct.

## Showcase Image

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203122113100.jpg)


## Ray/Triangle Intersection

To determine the intersection point of a ray with a triangle. We need to do two things:

+ Determine whether the ray intersects the plane containing the triangle.
+ If the intersection point exists, determine whether the point is inside the triangle.

### Ray/Plane Intersection

Let's use $p_0$, $p_1$, and $p_2$ to represent three verticesof the triangle.

Then we can define 

$$
e_1 = p_1 - p_0 \\
e_2 = p_2 - p_0 \\
n = (n_x, n_y, n_z) = e_1 \times e_2 \\
$$

Where $n$ is the surface normal to the plane. We can write the plane equation as
$$
Ax + By + Cz + D = 0
$$
Where $(A,B,C) = (n_x,n_y,n_z)$. Substitute the value a vertex into the equation above and we can solve for the unknown $D$.

Let the ray equation be expressed as
$$
x = x_0 + t x_d \\
y = y_0 + t y_d \\
z = z_0 + t z_d \\
$$
Substitute the ray equation into the plane equation and solve for $t$
$$
t = -(Ax_0 + By_0 + Cz_0 + D) / (Ax_d + By_d + Cz_d)
$$
When the denominator is zero, the ray and the plane are parallel with each other, thus no intersection. Otherwise when $t > 0$, they are intersected.

Let $p = (p_x, p_y, p_z)$ Represent the intersection point.

### Barycentric Coordinates

The tool we use to test whether a point is inside a triangle is called *barycentric coordinates*.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121237257.png)

As the image above shows, given a triangle and a point on the same plane, the point can be written as a linear combination of three vertexes of the triangle
$$
p &= p_0 + \beta (p_1 - p_0) + \gamma (p_2-p_0) \\
&= (1-\beta-\gamma)p_0 + \beta p_1 + \gamma p_2 \\
&= \alpha p_0 + \beta p_1 + \gamma p_2 \\
$$
Where $(\alpha, \beta, \gamma)$ is the barycentric coordinate of point $p$. It can be obtained by solving a simple linear system  equations.

We have the following rule to test whether a point is inside a triangle.

>Given the barycentric coordinate of a point $p$: ($\alpha, \beta, \gamma$), the point is inside the triangle is equivalent to the following conditions:
>
>+   $0 < \alpha < 1$
>+   $0 < \beta < 1$
>+   $0 < \gamma < 1$

## Flat Shading

To render triangles using flag shading, all points inside the triangle use the same surface normal which is the normal to the plane containing the triangle.

Given three verticesof a triangle $(p_0, p_1, p_2)$. The surface normal can be calculated as
$$
e_1 = p_1 - p_0 \\
e_2 = p_2 - p_0 \\
n = (n_x, n_y, n_z) = \frac{e_1 \times e_2}{||e_1 \times e_2||}\\
$$
Next we use this unit normal vector to evaluate the Blinn-Phong illumination equation.

The image below shows the effect of flat shading a triangle

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121330230.jpg)

## Smooth Shading

For smooth shading, the surface normal of a point inside the triangle is determined by interpolating from the three normal directions defined at the three triangle vertices. Using barycentric coordiantes as weights, namely
$$
p = \alpha p_0 + \beta p_1 + \gamma p_2 \\
n_{p} = \alpha n_{p_1} + \beta n_{p_2} + \gamma n_{p_3}
$$
Then we use this interpolated surface normal to evaluate the Blinn-Phong illumination equation.

The image below shows the effect of flat shading a triangle

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121330544.jpg)

## Texture Mapping

Next we map a texture image onto a object in the scene.

Texture coordiantes define the mapping between the pixels in a texture image, parametrized in a 2D space $(i,j)$, to points on a surface $(u,v)$. Namely we have the correspondence
$$
f: (i,j) \rightarrow (u,v)
$$
![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121333601.png)

The image above is an example texture image along with the texture coordinates. The top-left point has $(u,v)=(0,0)$, top-right has $(u,v) = (1,0)$, bottom-left $(u,v)=(0,1)$, bottom right $(u,v) = (1,1)$.

The program uses the texture coordinate of a point to retrieve the (diffuse) color from a texture image.

### Texture Mapping on a Sphere

To get the correspondence between a point on a sphere and a texture coordinate, we use the polar coordiante of a sphere
$$
x = x_c + r\cos \theta \sin \phi \\
y = y_c + r\sin \theta \sin \phi \\
z = z_c + r\cos \phi
$$
Where $r$ is the radius of the sphere which is a constant, $\theta \in (0,2\pi)$, $\phi \in (0,pi)$. We define a map between $(\theta, \phi)$ and $(u,v)$ as
$$
N = (N_x, N_y, N_z) \\
\theta = \atan(N_y/N_x) \\
\phi = \acos(N_z) \\
u = (\theta + \pi) / (2\pi) \\
v = \phi / pi
$$
Where $N=(N_x,N_y,N_z)$ is the unit surface normal vector at the ray/sphere intersection point.

Once we get the texture coordiante $(u,v)$,  we calculate the pixel coordinate in the texture image using bi-linear interpolation
$$
x = u\cdot (width-1) \\
y = v\cdot (height-1) \\
i = int(x); \alpha = x-i; j = int(y); \beta = y-j \\
color = (1-\alpha)(1-\beta)image(i,j) + (\alpha)(1-\beta)image(i+1,j) \\
+ (1-\alpha)(\beta)image(i,j+1) + (\alpha)(\beta)image(i+1,j+1)
$$
Bi-linear interpolation rather than nearest neighboor search has the advantage that it gives less blocky-looking results when the resolution of the texture is much lower than the resolution of the image being generated in the area around the point being rendered (referred as *texture maginification*). We could have use more advancede techiniques such as *MIP Mapping*.

Here is an example of texture mapping on spheres

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121352491.jpg)

### Texture Mapping on a Triangle

To map a texture image onto a triangle, we use barycentric coordinates again. Suppoing that we have
$$
p = \alpha p_0 + \beta p_1 + \gamma p_2
$$
And $(u_0, v_0)$, $(u_1, v_1)$, $(u_2,v_2)$ are three texture coordinates of three vertices separately. Then we can calcualte the texture coordinate of point $p$ as
$$
u = \alpha u_0 + \beta u_1 + \gamma u_2 \\
v = \alpha v_0 + \beta v_1 + \gamma v_2 \\
$$
Then we can do similar things to retrieve the color from the texture image as above.

Here is an example of texture mapping on triangles

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121404135.jpg)

## Extra Credit

### Normal Mapping

**The code for implementing normal mapping can be found in file `utils.h` and `utils.cpp`, in the function `normal_mapping`.**

For each object that uses texture mapping, it is also possible for the user to specify a normal map to modifies its normal direction. The normal map specifies a modification to the normal vector at a per-pixel level.

A normal map modifies the $(n_x, n_y, n_z)$ components of the unit surface normal vector that is used when computing the illumination at a point on a surface. To implement normal map, we need to consider differently for different types of objects.

#### Normal Mapping on Spheres

First for a ray/sphere intersection point, we can calculate its texture coordinate as described above. Once we have the texture coordinate, we convert it to pixel coordinate using neareat neighboor. Then we can retrieve the corresponding value in its normal map, namely, we have
$$
m = (m_x, m_y, m_z) = normal\_map(i, j)
$$
Where $(i,j)$ is the pixel coordinates of the point on the sphere.

Then we can use the following matrix to get the modified normal vector
$$
\begin{bmatrix}
n_x \\
n_y \\
n_z \\
\end{bmatrix}
=
\begin{bmatrix}
T_x & B_x & N_x \\
T_y & B_y & N_y \\
T_z & B_z & N_z \\
\end{bmatrix}
\begin{bmatrix}
m_x \\
m_y \\
m_z \\
\end{bmatrix}
$$
In the matrix equation above, $N = (N_x, N_y, N_z)$ is our original surface normal calcualted at point $p$. Vector $T$ is defined as
$$
T = (-\frac{N_y}{\sqrt{N_x^2+N_y^2}}, \frac{N_x}{\sqrt{N_x^2+N_y^2}}, 0)
$$
And vector $B$ is defined as
$$
B = N \times T = (-\frac{N_xN_z}{\sqrt{N_x^2+N_y^2}}, -\frac{N_yN_z}{\sqrt{N_x^2+N_y^2}}, \sqrt{N_x^2+N_y^2})
$$
Once we have $T$, $B$, and $N$. We can use the matrix transformation to get the modified normal vector $(n_x,n_y,n_z)$.

#### Normal Mapping on Triangles

The calculation for $T$ and $B$ are different if we want to apply normal mapping on triangles.
$$
\Delta u_1 = u_1 - u_0 \\
\Delta v_1 = v_1 - v_0 \\
e_1 = \Delta u_1 T + \Delta v_1 B = p_1 - p_0 \\
e_2 = \Delta u_2 T + \Delta v_2 B = p_2 - p_1 \\
\begin{bmatrix}
T \\
B \\
\end{bmatrix}

= \frac{1}{\Delta u_1 \Delta v_2 - \Delta v_1 \Delta u_2}
\begin{bmatrix}
\Delta v_2 & -\Delta v_1 \\
-\Delta u_2 & \Delta u_1 \\
\end{bmatrix}
\begin{bmatrix}
e_1 \\
e_2 \\
\end{bmatrix}
$$
Once we have $T$, $B$, and $N$, we can transform the normal vector.

Here is an example applying normapping to both spheres and triangles in the scene:

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121621393.jpg)

Here is the texture image

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121626475.jpg)

Here is my normal map

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202203121625925.jpg)

Because I don't really know how to generate a proper normal map for a texture image. I just create one randomly.

## Known Issues

None

## Credits

Zecheng Qian (qian0102@umn.edu)
