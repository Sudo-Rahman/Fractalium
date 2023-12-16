
#ifndef FRACTALIUM_COMPLEX_HPP
#define FRACTALIUM_COMPLEX_HPP


#include <Double.hpp>

namespace Fractalium {

    class Complex {
    public:
        Double real, imag;

        Complex(Double r, Double i);

        Complex();

        Complex &operator=(Complex const &obj);

        Complex operator+(Complex const &obj) const;

        Complex operator-(Complex const &obj) const;

        Complex operator*(Complex const &obj) const;

        Complex operator/(Complex const &obj) const;

        Complex &operator+=(Complex const &obj);

        Complex &operator-=(Complex const &obj);

        Complex &operator*=(Complex const &obj);

        Complex &operator/=(Complex const &obj);

        [[nodiscard]] Double norm() const;
    };
}
#endif //FRACTALIUM_COMPLEX_HPP
