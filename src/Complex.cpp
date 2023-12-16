#include "Complex.hpp"

using namespace Fractalium;

/**
 * @brief  Constructeur par défaut
 * @param  r: partie réelle du nombre complexe
 * @param  i: partie imaginaire du nombre complexe
 */
Complex::Complex(Double r, Double i)
{
    this->real = std::move(r);
    this->imag = std::move(i);
}

/**
 * @brief  Constructeur par copie
 */
Complex::Complex()
{
    this->real = 0;
    this->imag = 0;
}

/**
 * @brief  Retourne le module du nombre complexe
 * @return Double
 */
Double Complex::norm() const
{
    return sqrt(real * real + imag * imag);
}

/**
 * @brief  Retourne le carré du module du nombre complexe
 * @return Complex

 */
Complex &Complex::operator=(const Complex &obj)
{
    if (this != &obj) { // Vérifier l'auto-affectation
        real = obj.real;
        imag = obj.imag;
    }
    return *this;
}

/**
 * @brief  Surcharge de l'opérateur d'addition
 * @param obj  Nombre complexe à ajouter
 * @return  Complex
 */
Complex Complex::operator+(Complex const &obj) const
{
    return {real + obj.real, imag + obj.imag};
}

/**
 * @brief  Surcharge de l'opérateur de soustraction
 * @param obj  Nombre complexe à soustraire
 * @return  Complex
 */
Complex Complex::operator-(Complex const &obj) const
{
    return {real - obj.real, imag - obj.imag};
}

/**
 * @brief  Surcharge de l'opérateur de multiplication
 * @param obj  Nombre complexe à multiplier
 * @return  Complex
 */
Complex Complex::operator*(Complex const &obj) const
{
    return {real * obj.real - imag * obj.imag, real * obj.imag + imag * obj.real};
}


/**
 * @brief  Surcharge de l'opérateur de division
 * @param obj  Nombre complexe à diviser
 * @return  Complex
 */
Complex Complex::operator/(Complex const &obj) const
{
    return {(real * obj.real + imag * obj.imag) / (obj.real * obj.real + obj.imag * obj.imag),
            (imag * obj.real - real * obj.imag) / (obj.real * obj.real + obj.imag * obj.imag)};
}

/**
 * @brief  Surcharge de l'opérateur de comparaison
 * @param obj  Nombre complexe à comparer
 * @return  bool
 */
Complex& Complex::operator+=(const Complex &obj)
{
    *this = this->operator+(obj);
    return *this;
}

/**
 * @brief  Surcharge de l'opérateur de comparaison
 * @param obj  Nombre complexe à comparer
 * @return  bool
 */
Complex &Complex::operator-=(const Complex &obj)
{
    *this = this->operator-(obj);
    return *this;
}

/**
 * @brief  Surcharge de l'opérateur de comparaison
 * @param obj  Nombre complexe à comparer
 * @return  bool
 */
Complex& Complex::operator*=(const Complex &obj)
{
    *this = this->operator*(obj);
    return *this;
}

/**
 * @brief  Surcharge de l'opérateur de comparaison
 * @param obj  Nombre complexe à comparer
 * @return  bool
 */
Complex &Complex::operator/=(const Complex &obj)
{
    *this = this->operator/(obj);
    return *this;
}