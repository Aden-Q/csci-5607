# CSCI 5607 Assignment 1a

## Usage

+ Enter the `src` foler
+ `make`
+ `./raytracer path_to_`

## Description

The program reads scene description from a file (as a command line argument). Then it runs a ray tracing algorithm and create an image in ppm format.

## File Organization

+ `src`: contains all the source code and makefile.
+ `input`: scene description input file for testing.
+ `output`: some examples the output produced by the program.

## Implementation Details

1. Construct the scene using `parse_scene`.
2. Construct a viewing window using `view_window_init`.
3. For each pixel in the image, run a ray tracing algorithm using `trace_ray`.
4. For each ray, checking whether it intersects with any object in the scene, using `intersect_check`. When intersecting, get the corresponding color of that object and render the pixel.
5. Once all pixels in the image are rendered, produce an output image in `PPM` format.

## Questions to Answer

### How does the apparent rotation of the scene with respect to the viewpoint change with changes in the direction of the ‘up’ vector?

First we get an image with $up = <0,1,0>$ where the up vector points upwards, with the provided sample input 1:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0ub1r3f6j30e80e8jrd.jpg)

When I change the view direction from $<0,1,0>$ to $<1,0,0>$: rotate 90 degrees about the viewing direction, the image also rotates by 90 degrees, as shown below:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0uhki0y4j30e80e8jrd.jpg)

When I change the view direction to be $up = <0,1,1>$, which means now the up direction is more aligned with the viewing direction. I get an image:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0ulewy6qj30e80e8jrd.jpg)

We can see this image is same as the original one because the $u$ and $v$ vector does not change in the viewing window in this case.

### How do changes in the field of view settings affect the appearance of the scene in your rendered image?

When I change the vertical field of view from 60 degrees to 30 degrees (the view is narrowed down both vertically and horizontally because the aspect ratio must match), I get an image as following:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0uplqw2pj30e80e8746.jpg)

When I set the vertical field of view to be 180 degrees (in the case we have a larger viewing window if the viewing distance ramains the same):

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0ur1n6r3j30e80e8glg.jpg)

### How can the viewing parameters (e.g. the camera location, field of view settings, …) be adjusted to achieve a less exaggerated vs more exaggerated amount of apparent perspective distortion in your image?

When I set the view direction to point to the center of the first sphere (set $vdir = <3,3,10>$) and leave other settings as default, I get this image (less distortion):

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0uwhxo2tj30e80e8mx2.jpg)

When I move my eye toward the spheres, tilde up my view direction a little and make the field of view larger, I get a more distorted image:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0vdmt762j30e80e8gli.jpg)

Similarly, I can also distorted the other sphere:

![](https://tva1.sinaimg.cn/large/008i3skNgy1gz0veb9u95j30e80e8q2u.jpg)

## Credits

Zecheng Qian (qian0102@umn.edu)
