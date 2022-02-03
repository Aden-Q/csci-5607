# CSCI 5607 Assignment 1a

## Usage

+ Enter the `src` foler
+ `make`
+ `./raytracer filename`

## Description

The program reads scene description from a file (as a command line argument). Then it runs a ray tracing algorithm and create an image in ppm format.

## File Organization

+ `src`: contains all the source code and makefile.
+ `input`: scene description input file for testing.
+ `output`: some examples the output produced by the program.

## Implementation Details

I create a subroutine named `generate_cat` which uses the opencv library and will take a non-ppm format image and generate a ppm format image for it. I intentionally comment out the code block from line 4, line 16-31 and line 100 just for testing purpose. (I attach a cat image for testing as well!)

Command to compile with opencv library:

+ `g++ $(pkg-config --cflags --libs opencv4) -std=c++11  simpleppm.cpp -o simpleppm`

Then run as usual.

The folder named `src` contains the source code, makefile and the image file I use to create cat image in ppm format with OpenCV.

The folder named `output` contains several output images by my program.

## Questions to Answer

### How does the apparent rotation of the scene with respect to the viewpoint change with changes in the direction of the ‘up’ vector?

First we get an image with $up = <0,1,0>$ where the up vector points upwards, with the provided sample input 1:

```html
<img src="src/image/README/output1.png" alt="Getting started" />
```




### How do changes in the field of view settings affect the appearance of the scene in your rendered image?



### How can the viewing parameters (e.g. the camera location, field of view settings, …) be adjusted to achieve a less exaggerated vs more exaggerated amount of apparent perspective distortion in your image?


## Credits

Zecheng Qian (qian0102@umn.edu)
