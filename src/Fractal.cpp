//
// Created by sr-71 on 28/11/2023.
//

#include "Fractal.hpp"
#include <boost/thread.hpp>

using namespace Fractalium;

const int Fractal::ITERATIONS = 100;

std::pair<Double, Double> Fractal::_offset = {-2.1, -2};

Complex Fractal::Julia::juliaConstant = {0.285, 0.01};

int Fractal::pointCheck(const Complex &point, int iterations) const
{
    switch (_type)
    {
        case FractalType::Mandelbrot:
            return Mandelbrot::pointCheck(point, iterations);
        case FractalType::Julia:
            return Julia::pointCheck(point, iterations);
    }
    return 0;
}

int Fractal::Julia::pointCheck(const Complex &point, int iterations)
{
    Complex z = point;
    int i;
    for (i = 0; i < iterations; ++i)
    {
        boost::this_thread::interruption_point();
        z *= z += Fractal::Julia::juliaConstant;
        if (z.norm() > 2.0)
            break;
    }
    return i;
}

int Fractal::Mandelbrot::pointCheck(const Complex &point, int iterations)
{
    int i;
    Complex z = Complex(0, 0);
    for (i = 0; i < iterations; ++i)
    {
        boost::this_thread::interruption_point();
        z *= z += point;
        if (z.norm() > 2.0)
            break;
    }
    return i;
}
