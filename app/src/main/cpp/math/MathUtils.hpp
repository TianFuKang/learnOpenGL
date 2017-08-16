#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>

//#include "Matrix.hpp"

namespace mj2 {

    enum Axis
    {
        Axis_X = 0, Axis_Y = 1, Axis_Z = 2
    };

    template<class T> constexpr T pi = 3.14159265358979323846264338327;
    template<class T> constexpr T two_pi = 6.28318530717958647692528676656;
    template<class T> constexpr T half_pi = pi<T> * 0.5;
    template<class T> constexpr T degree_to_rad_fator = two_pi<T> / 360.0;

    template<class T> constexpr inline T DegreeToRad(T deg)
    {
        return deg * degree_to_rad_fator<T>;
    }

    const float  PI_F = 3.14159265358979f;

    static inline float InvSqrt(float f)
    {
        // TODO: quake 3, neon
        return std::sqrt(1.0f / f);
    }

    struct Range {
    public:
        float min;
        float max;

    public:
        inline float length() { return (max - min); }
    };

    static inline float MapRange(float x, Range fromRange, Range toRange)
    {
        return (x - fromRange.min) * toRange.length() / fromRange.length() + toRange.min;
    }


    /////////////////////


} // end of namespace mj2
