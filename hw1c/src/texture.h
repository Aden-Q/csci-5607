/**
 * @file texture.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_

#include <cstdlib>
#include "color.h"

    class Texture
{
    public:
        // constructor
        Texture(int width = 512, int height = 512, int max_val = 255, Color **checkerboard = NULL)
        {
            this->width = width;
            this->height = height;
            this->max_val = max_val;
            // initialize an empty checker board with size w*h
            this->checkerboard = new Color *[width];
            for (int i = 0; i < width; i++)
            {
                this->checkerboard[i] = new Color[height];
            }
        }

        // getter
        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }
        int getMaxVal() const { return this->max_val; }
        Color **getCheckerboard() const { return this->checkerboard; }
        const Color &getColor(int i, int j) const { return this->checkerboard[i][j]; }
        
        // setter
        void setWidth(int width) { this->width = width; }
        void setHeight(int height) { this->height = height; }
        void setMaxVal(int max_val) { this->max_val = max_val; }

    private:
        // size of the texture image
        int width, height;
        // maximum color value
        int max_val;
        // dynamically allocate a 2d array to store pixels in the image
        Color **checkerboard;
};

#endif // SRC_TEXTURE_H_
