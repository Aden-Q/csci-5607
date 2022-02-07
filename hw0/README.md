# CSCI 5607 Assignment 0

## Usage

+ `make`
+ `./simpleppm filename`

## Description

The program reads image description from a file (as a command line argument) in the format "imsize width height". Then it creates an image in ppm format with the width and height as indicated above.

## Aditional Note

I create a subroutine named `generate_cat` which uses the opencv library and will take a non-ppm format image and generate a ppm format image for it. I intentionally comment out the code block from line 4, line 16-31 and line 100 just for testing purpose. (I attach a cat image for testing as well!)

Command to compile with opencv library:

+ `g++ $(pkg-config --cflags --libs opencv4) -std=c++11  simpleppm.cpp -o simpleppm`

Then run as usual.

The folder named `src` contains the source code, makefile and the image file I use to create cat image in ppm format with OpenCV.

The folder named `output` contains several output images by my program.

## Credits

Zecheng Qian (qian0102@umn.edu)
