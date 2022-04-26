# CSCI 5607 Assignment 2b: Learning About Viewing, Projection and Viewport Transformations via a First-Person 3D Walkthrough

## Description

This program can interactively simulates a first-person view of walking through a 3D scene.

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

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251930914.png)

### CMake-CLI

TBD

## Usage

Run the executable. The functionalities are listed as following (trigger by keyboard events). `Callbacks` are binded with the events.

+   `W`: Move forward, view direction unchanged

+   `S`: Move backward, view direction unchanged
+   `A`: Move to the left, view direction unchanged
+   `D`: Move to the right, view direction unchanged
+   `[`: Move up, view direction unchanged
+   `]`: Move down, view direction unchanged
+   $\leftarrow$: Rotate the viewing direction to the left
+   $\rightarrow$: Rotate the viewing direction to the right
+   $\uparrow$: Rotate the viewing direction up
+   $\downarrow$: Rotate the viewing direction down

## Demo

**Static view**:

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251414275.png)

**Walking through the scene interactively**:

<img src=https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251934287.gif alt="drawing" width="500"/>

## Implementation Details

### Viewing Transformation

The purpose of **viewing transformation** is to change from the <u>world coordinates</u> into <u>viewing coordinates</u>.

![](https://raw.githubusercontent.com/Aden-Q/blogImages/main/img/202204251126609.png)

As the image above shows, when transforming vertices from world coordinates into viewing coordinates, we want to:

+   Translate so that the position of the camera aligns with the world origin
+   Rotate so that the axises in the camera's coordinate system align with the axises in the world coordinate system

There are mainly three steps to achieve both goals:

1.   Translation such that the camera centers at the orgin

2.   Do a rotation such that the $xyz$ world coordinate frame is in alignment with the $uvn$ viewing coordinate frame in the camera coordinate system
3.   Composite the two transformations

The **first step** can be done by a simple translation matrix:
$$
T = 
\begin{bmatrix}
1 & 0 & 0 & -e_x \\\\
0 & 1 & 0 & -e_y \\\\
0 & 0 & 1 & -e_z \\\\
0 & 0 & 0 & 1 \\\\
\end{bmatrix}
$$
The **second step** can be done by a simple rotation matrix:
$$
V = 
\begin{bmatrix}
u_x & u_y & u_z & 0 \\\\
v_x & v_y & v_z & 0 \\\\
n_x & n_y & n_z & 0 \\\\
0 & 0 & 0 & 1 \\\\
\end{bmatrix}
$$
The final composite **viewing transformation** matrix (by the third step) has the following form:
$$
V = R \cdot T = 
\begin{bmatrix}
u_x & u_y & u_z & d_x \\\\
v_x & v_y & v_z & d_y \\\\
n_x & n_y & n_z & d_z \\\\
0 & 0 & 0 & 1 \\\\
\end{bmatrix}
$$

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
1 & 0 & 0 & 0 \\\\
0 & 1 & 0 & 0 \\\\
0 & 0 & -1 & 0 \\\\
0 & 0 & 0 & 1 \\\\
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
\frac{2\cdot near}{right-left} & 0 & \frac{right+left}{right-left} & 0 \\\\
0 & \frac{2 \cdot near}{top-bottom} & \frac{top+bottom}{top-bottom} & 0 \\\\
0 & 0 & -\frac{far + near}{far - near} & - \frac{2 \cdot far \cdot near}{far -near} \\\\
0 & 0 & -1 & 0 \\\\
\end{bmatrix}
$$

## Moving Around and Adjusting View Direction

**Moving Around**:

The camera position can be adjusted by keys `W`, `S`, `A`, `D`, to move forward, backward, to the left, and to the right seperately. When the keyboard event is triggered, we can simply update our **viewing transformation matrix** because several entries of the matrix are affected by the eye position. Namely, we re-calculated 3 entries $(d_x, d_y, d_z)$ of the following matrix:
$$
V = 
\begin{bmatrix}
u_x & u_y & u_z & d_x \\\\
v_x & v_y & v_z & d_y \\\\
n_x & n_y & n_z & d_z \\\\
0 & 0 & 0 & 1 \\\\
\end{bmatrix}
$$

$$
d_x = -eye \cdot u \\\\
d_y = -eye \cdot v \\\\
d_z = -eye \cdot n
$$

The update is efficient because we only need to deal with three entries of the 4x4 matrix.

**Adjusting View Direction**

The viewing direction can be adjusted by keys $\leftarrow$, $\rightarrow$, $\uparrow$, $\downarrow$, to rotate counterclockwise, clockwise, up, and down seperately. When the keyboard event is triggered, we can also update our **viewing transformation matrix** becuase several entries of the matrix are affected by the viewing direction. Namely, we re-calculated 12 entries of the following matrix:
$$
V = 
\begin{bmatrix}
u_x & u_y & u_z & d_x \\\\
v_x & v_y & v_z & d_y \\\\
n_x & n_y & n_z & d_z \\\\
0 & 0 & 0 & 1 \\\\
\end{bmatrix}
$$

$$
n = -view\_dir \\\\
u = up \cross n \\\\
v = n \cross u \\\\
d_x = -eye \cdot u \\\\
d_y = -eye \cdot v \\\\
d_z = -eye \cdot n
$$

## Resizing Window

When we resize the window, we want the projected scene to be adjusted as well. We implement this by first resizing the viewport according to the window size, then adjust the **viewing frustum** and the **perspective projection matrix** accordingly.

We use the *aspect_ratio* as a criteria to adjust everything:
$$
aspect\_ratio = width / height
$$
We make sure that the *asepect_ratio* of the window, viewing frustum, and the viewport all align. So that we will not see any distortions when we resize the window.

## Extra Credit

### Rotate Up and Down

This functionality allows the user to adjust the viewing direction up and down consistently even though we flip 180 degrees.

+   $\uparrow$: Rotate the viewing direction upward
+   $\downarrow$: Rotate the viewing direction downward

After adjusting the viewing direction, we simply update the **viewing transformation matrix** and redraw the scene. To make everything consistent, we need to rotate abot the viewing direction.

The rotation along an arbitrary direction $\vec a$ is done by doing the following steps:

1.   Translate by a vector $t$ such that $\vec a$ origins at the $(0, 0, 0)$ origin
2.   Rotate by a matrix $R$ such that $\vec a$ coincide with the negative $z$ axis
3.   Perform the rotation about the $x$ axis
4.   Apply the inverse rotation matrix $R^T$
5.   Apply the inverse translation matrix

### Move Up and Down

This functionality allows the user to adjust the camera position up and down consistently (**along the `up` direction instead of the `v` direction**)

+   `[`: Move up, view direction unchanged
+   `]`: Move down, view direction unchanged

After adjusting the camera position, we simply update the **viewing transformation matrix** and redraw the scene.

## Known Issues

Not found yet

## Credits

Zecheng Qian (qian0102@umn.edu)
