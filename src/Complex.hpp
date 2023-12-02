
#ifndef FRACTALIUM_COMPLEX_HPP
#define FRACTALIUM_COMPLEX_HPP


#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

namespace Fractalium
{


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
}
#endif //FRACTALIUM_COMPLEX_HPP
