//
// Created by sr-71 on 28/11/2023.
//

#include "Fractal.hpp"

int Mandelbrot::pointCheck(const Complex &point, int iterations) const
{
    int i;
    Complex z = Complex(0, 0);
    for (i = 0; i < iterations; ++i)
    {
        z = z * z + point;
        if (z.pointRadius() > 2.0)
        {
            break;
        }
    }
    return i;
}

int Julia::pointCheck(const Complex &point, int iterations) const
{
    Complex z = point;
    int i;
    for (i = 0; i < iterations; ++i)
    {
        z = z * z + juliaConstant;
        if (z.pointRadius() > 2.0)
        {
            break;
        }
    }
    return i;
}
