# CSCI 5607 Assignment 2a: Programming Interactive 2D Graphics with OpenGL

## Description

This program implements simple 2D geometric transformation in a vertex shader to allow a user to interactively reposition and resize a single object in a window via the mouse and keyboard. A single window and single 2D polygon object are supported.

## Dependencies

+   `cmake`
+   `opengl >= 3.2`
+   `glfw` (provided)

## File Organization

+ `src`: all the source code input files
+ `assets`: some simple demo of the the program 
+ `ext`: external library
+ `bin`: built binaries including the executable
+ `CMakeLists.txt`: CMake description file for building purpose

## Build

### CMake-GUI

+   Specify the source code directory to be the project folder
+   Specify where to build the binaries
+   Click `Configure`
+   Click `Generate`
+   Navigate to the directory where the project is built
+   Build the project

An example is showns below

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204100108892.png)

### CMake-CLI

TBD

## Usage

Run the executable. The functionalities are listed as following (trigger by keyboard events and mouse events). Each time an event is triggered, the object will be repositioned and replotted in the display window. `Callbacks` are binded with the events that we want to capture.

+   &#8592;：Make the object thinner
+   &#8593;: Make the object taller
+   &#8594;: Make the object fatter
+   &#8595;: Make the object shorter
+   `space`: Restore the object to its origital position
+   Moving the mouse from left to right while pressing the left button: Clockwise rotation
+   Moving the mouse from rightto left while pressing the left button: Counterclockwise rotation
+   Moving the mouse while the left button and the `Control` key are simultaneously pressed down: Translation along the moving direction of the mouse.

## Demo

An example of a pentagon:

![demo](./assets/demo.gif)

An example of a square:

![demo](./assets/demo2.gif)

## Implementation Details

A 4x4 transformation matrix is used by the vertex shader to transform and render a single vertex.
$$
M = 
\begin{bmatrix}
m_0 & m_4 & m_8 & m_{12} \\
m_1 & m_5 & m_9 & m_{13} \\
m_2 & m_6 & m_{10} & m_{14} \\
m_3 & m_7 & m_{11} & m_{15} \\
\end{bmatrix}
$$
In the program, the transformation matrix $M$ is defiend as a one dimensional array, specified in column-major order. It is applied on a vector of 4 elements, using **homogeneous coordinates**. For a 3D point $(x,y,z)$, its homogeneous coordinates is a $(x_h,y_h,z_h, h)$ with the following constraints:
$$
h>0 \\
x = \frac{x_h}{h} \\
y = \frac{y_h}{h} \\
z = \frac{z_h}{h} \\
$$
Since homogeneous coordiantes are not uniquely defined, typically we choose $h=1$ for simplicity. In the following, if not specified otherwise, we are always using the notation of homogeneous coordinates.

### Scaling

For a 3D point $(x,y,z,1)$, the scaling transformation can be written as
$$
\begin{bmatrix}
x' \\
y' \\
z' \\
1 \\
\end{bmatrix}
=
\begin{bmatrix}
s_x & 0 & 0 & 0 \\
0 & s_y & 0 & 0 \\
0 & 0 & s_z & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
x \\
y \\
z \\
1 \\
\end{bmatrix}
$$
Where $s_x$ is the streching factor along the $x$ axis, $s_y$ is the streching factor along the $y$ axis, and $s_z$ is the streching factor along the $z$ axis. 

For 2D scaling in the `xy` plane, we have $s_z = 0$. In this program, when there is a composite transformation, the scaling is enforced to be performed along the object's intrinct axises, making sure there is no shearing effects.

### Rotation

For a 3D point $(x,y,z,1)$, a 2D rotation transformation can be written as
$$
\begin{bmatrix}
x' \\
y' \\
z' \\
1 \\
\end{bmatrix}
=
\begin{bmatrix}
\cos(\theta) & -\sin(\theta) & 0 & 0 \\
\sin(\theta) & \cos(\theta) & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
x \\
y \\
z \\
1 \\
\end{bmatrix}
$$
It represents a counterclockwise rotation by an angle $\theta$ about the pivot point. The pivot point is chosen to be the centroid of the display window, which has a coordinate of $(0,0,0,1)$.

### Translation

For a 3D point $(x,y,z,1)$, a 2D translation transformation can be written as
$$
\begin{bmatrix}
x' \\
y' \\
z' \\
1 \\
\end{bmatrix}
=
\begin{bmatrix}
1 & 0 & 0 & t_x \\
0 & 1 & 0 & t_y \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
\begin{bmatrix}
x \\
y \\
z \\
1 \\
\end{bmatrix}
$$
Where $t_x$ and $t_y$ represent the translation amount along the $x$ axis and $y$ axis, respectively.

## Extra Credit

The program can read an object from a `txt` file. Two examples are provided named `scene1.txt` and `scene2.txt` under the `src` folder.

The program is capable of dealing with a polygon using a scene description file with up to 100 vertices.

An example of a pentagon is shown above. The program support 2D transformation including scaling, rotation about the centroid and translation.

The scene description file should specify each vertex on each row using the following syntax:

```
v x y z r g b
```

`(x,y,z)` specifies the 3D location of the vertex, and  `(r,g,b)` specifies the color associated with the vertex.

Since the object is 2D, the `z` component is not used but required in the scene description file as a placeholder.

**Note: Be sure to modify line 285 in `HW2.cpp` to try different scene description files.**

## Known Issues

Not found yet

## Credits

Zecheng Qian (qian0102@umn.edu)
