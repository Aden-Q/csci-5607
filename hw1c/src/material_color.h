/**
 * @file material_color.h
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#ifndef SRC_MATERIAL_COLOR_H_
#define SRC_MATERIAL_COLOR_H_

#include "color.h"

class MaterialColor
{
    public:
        // default constructor
        MaterialColor()
        {
            this->ka = 0;
            this->kd = 1;
            this->ks = 0;
            this->n = 1;
        }
        MaterialColor(Color Od_lambda, Color Os_lambda, float ka, float kd, float ks, float n)
        {
            this->Od_lambda = Color(Od_lambda);
            this->Os_lambda = Color(Os_lambda);
            this->ka = ka;
            this->kd = kd;
            this->ks = ks;
            this->n = n;
        }

        // getter
        const Color &getOd() const { return this->Od_lambda; }
        const Color &getOs() const { return this->Os_lambda; }
        float getKa() const { return this->ka; }
        float getKd() const { return this->kd; }
        float getKs() const { return this->ks; }
        float getN() const { return this->n; }

        // setter
        void setOd(const Color &Od_lambda) { this->Od_lambda = Color(Od_lambda); }
        void setOs(const Color &Os_lambda) { this->Os_lambda = Color(Os_lambda); }
        void setKa(float ka) { this->ka = ka; }
        void setKd(float kd) { this->kd = kd; }
        void setKs(float ks) { this->ks = ks; }
        void setN(float n) { this->n = n; }

    private:
        Color Od_lambda;  // intrinsic color
        Color Os_lambda;  // diffuse color
        float ka, kd, ks;
        float n;
};

#endif // SRC_MATERIAL_COLOR_H_
