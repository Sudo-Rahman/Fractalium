//
// Created by sr-71 on 28/11/2023.
//

#ifndef FRACTALIUM_FRACTAL_HPP
#define FRACTALIUM_FRACTAL_HPP

#include <Complex.hpp>


namespace Fractalium
{

    class Fractal {


    public:
        [[nodiscard]] virtual int pointCheck(const Complex &pointm, int iterations) const = 0;

        static const int ITERATIONS;

        static std::pair<Double, Double> _offset;
    };

    class Mandelbrot : public Fractal {

    public:
        [[nodiscard]] int pointCheck(const Complex &point, int iterations) const override;
    };

    class Julia : public Fractal {

        Complex juliaConstant{0.285, 0.01};
    public:
        explicit Julia(Complex constant) : juliaConstant(std::move(constant))
        {}

        explicit Julia() = default;

        [[nodiscard]] int pointCheck(const Complex &point, int iterations) const override;
    };
}

#endif //FRACTALIUM_FRACTAL_HPP
