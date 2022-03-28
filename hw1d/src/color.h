/**
 * @file color.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

class Color
{
    public:
        // constructor
        Color(float r = 0, float g = 0, float b = 0)
            : r(r), g(g), b(b)
        {
        }

        // getter
        float getR() const { return this->r; }
        float getG() const { return this->g; }
        float getB() const { return this->b; }

        // setter
        void setR(float r) { this->r = r; }
        void setG(float g) { this->g = g; }
        void setB(float b) { this->b = b; }

        // operater overload
        // overload + operator, addition
        Color operator+(const Color &c) const
        {
            return Color(this->r + c.r, this->g + c.g, this->b + c.b);
        }

        // overload * operator, const multiple
        Color operator*(float c) const
        {
            return Color(this->r * c, this->g * c, this->b * c);
        }

    private:
        // R, G, B components of a color, in the range 0-1
        float r, g, b;
};

#endif // SRC_COLOR_H_
