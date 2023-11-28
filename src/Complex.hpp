#ifndef MANDELBROT_COMPLEX_HPP
#define MANDELBROT_COMPLEX_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

typedef cpp_dec_float_100 Double;

class Complex {
public:
    Double real, imag;

    Complex(Double r, Double i);

    Complex();

    Complex operator+(Complex const &obj) const;

    Complex operator-(Complex const &obj) const;

    Complex operator*(Complex const &obj) const;

    Complex operator/(Complex const &obj) const;

    [[nodiscard]] Double pointRadius() const;
};

#endif //MANDELBROT_COMPLEX_HPP
