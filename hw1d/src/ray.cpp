/**
 * @file ray.cpp
 *
 * @copyright 2022 Zecheng Qian, All rights reserved.
 */
#include "ray.h"

FloatVec3 Ray::extend(float t) const
{
    return FloatVec3(this->center.first + t * this->dir.first,
                     this->center.second + t * this->dir.second,
                     this->center.third + t * this->dir.third);
}
