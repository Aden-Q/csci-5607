# CSCI 5607 Assignment 2b: Learning About Viewing, Projection and Viewport Transformations via a First-Person 3D Walkthrough

## Description

This program simulates a first person view of walking around a scene.

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

**Static view**:

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251414275.png)

**Walking through the scene interactively**:









## Implementation Details

**Global Configurations**

+   view origin:
+   view direction:
+   up direction:



### Viewing Transformation

The purpose of **viewing transformation** is to change from the <u>world coordinates</u> into <u>viewing coordinates</u>.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251126609.png)

As the image above shows, when transforming vertices from world coordinates into viewing coordinates, we want to:

+   Translate so that the position of the camera aligns with the world origin
+   Rotate so that the axises in the camera's coordinate system align with the axises in the world coordinate system

There are mainly three steps to achieve both goals:

1.   TBD
2.   TBD
3.   TBD









### Projection Transformation

The purpose of **projection transformation** is to map all of the <u>vertices</u> in the scene from <u>camera coordinates</u> into <u>normalized clipping coordinates</u>. This is a following process after **viewing transformation**, so we have our eye origin at $(0, 0, 0)$ in the camera coordinate system.

There are main categories of projection from 3D to 2D:

+   Parallel projection: All points in a scene are projected from 3D to 2D using the <u>same</u> projection direction.
    +   Orthogrtaphic/Orthogonal projection: All points are projected from 3D to 2D along a constant direction that is orthogonal to the projection plane.
    +   Oblique: All points are projected from 3D to 2D along a constant direction that is <u>not</u> orthogonal to the projection plane, but is instead oriented at an angle with respect to the projection plane.
+   Perspective projection: The direction of projection between each individual point in space and the projection plane may be different..

A basic orthographic parallel projection can be written as:
$$
M_{ortho} = 
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & -1 & 0 \\
0 & 0 & 0 & 1 \\
\end{bmatrix}
$$
Although parallel projections are easy to implement, they do not provide realistic views. To obtain a realistic view, we use a **perspective projection** in our implementaion.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251228706.png)

We call the volumn in grey **View Volume** or **View Frusum**, in the following when we refer to this volumn using two names, basically they refer to the same thing.

A perspective projection view volume can be defined by six bounding planes **left, right, bottom, right, near, far**. It's important to enforce **near** > 0 so that everything is in front of the eye.

----

For simplicity, we implement a **symmetric perspective projection** in which we have

+   The vector between the center of projection (our eye), and the center of the viewing window is orthogonal to the viewing window.
+   The boundaries **(left, right)**, **(bottom, up)** are pairwise symmetric.

The image below is a simple illustration of **symmetric perspective projection**:

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251242552.png)

**Symmetric persepective projection** can be implemented in two steps:

+   A *warping* operation to transform the trapezoidal volume into a rectangular volume (for clipping purpose which will be explained later)
+   *Normalization* into a unit cube

The composite of these two steps can be written as a single transformation matrix:
$$
M_{persp} = 
\begin{bmatrix}
\frac{2\cdot near}{right-left} & 0 & \frac{right+left}{right-left} & 0 \\
0 & \frac{2 \cdot near}{top-bottom} & \frac{top+bottom}{top-bottom} & 0 \\
0 & 0 & -\frac{far + near}{far - near} & - \frac{2 \cdot far \cdot near}{far -near} \\
0 & 0 & -1 & 0 \\
\end{bmatrix}
$$




## Extra Credit





## Known Issues

Not found yet

## Credits

Zecheng Qian (qian0102@umn.edu)
