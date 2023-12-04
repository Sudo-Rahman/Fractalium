#include "Complex.hpp"

using namespace Fractalium;

Complex::Complex(Double r, Double i)
{
    this->real = std::move(r);
    this->imag = std::move(i);
}

Complex::Complex()
{
    this->real = 0;
    this->imag = 0;
}

Double Complex::norm() const
{
    return sqrt(real * real + imag * imag);
}

Complex &Complex::operator=(const Complex &obj)
{
    if (this != &obj) { // Vérifier l'auto-affectation
        real = obj.real;
        imag = obj.imag;
    }
    return *this;
}


Complex Complex::operator+(Complex const &obj) const
{
    return {real + obj.real, imag + obj.imag};
}

Complex Complex::operator-(Complex const &obj) const
{
    return {real - obj.real, imag - obj.imag};
}

Complex Complex::operator*(Complex const &obj) const
{
    return {real * obj.real - imag * obj.imag, real * obj.imag + imag * obj.real};
}


Complex Complex::operator/(Complex const &obj) const
{
    return {(real * obj.real + imag * obj.imag) / (obj.real * obj.real + obj.imag * obj.imag),
            (imag * obj.real - real * obj.imag) / (obj.real * obj.real + obj.imag * obj.imag)};
}

Complex& Complex::operator+=(const Complex &obj)
{
    *this = this->operator+(obj);
    return *this;
}

Complex &Complex::operator-=(const Complex &obj)
{
    *this = this->operator-(obj);
    return *this;
}

Complex& Complex::operator*=(const Complex &obj)
{
    *this = this->operator*(obj);
    return *this;
}

Complex &Complex::operator/=(const Complex &obj)
{
    *this = this->operator/(obj);
    return *this;
}