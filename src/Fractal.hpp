//
// Created by sr-71 on 28/11/2023.
//

#ifndef MANDELBROT_FRACTAL_HPP
#define MANDELBROT_FRACTAL_HPP

#include <utility>

#include "Complex.hpp"

class Fractal {

public:
    [[nodiscard]] virtual int pointCheck(const Complex &pointm, int iterations) const = 0;
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

#endif //MANDELBROT_FRACTAL_HPP
