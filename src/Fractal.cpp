//
// Created by sr-71 on 28/11/2023.
//

#include "Fractal.hpp"

using namespace Fractalium;

Complex Fractal::Julia::juliaConstant = {0.285, 0.01};

/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans un ensemble de fractales
 * @param point
 * @param iterations
 * @return
 */
int Fractal::pointCheck(const Complex &point, const int iterations) const {
    switch (_type) {
        case FractalType::Mandelbrot:
            return Mandelbrot::pointCheck(point, iterations);
        case FractalType::Julia:
            return Julia::pointCheck(point, iterations);
        case FractalType::BurningShip:
            return BurningShip::pointCheck(point, iterations);
        case FractalType::Newton:
            return Newton::pointCheck(point, iterations);
        case FractalType::Koch:
            return Koch::pointCheck(point, iterations);
    }
    return 0;
}

/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans l'ensemble de Julia
 * @param point
 * @param iterations
 * @return
 */
int Fractal::Julia::pointCheck(const Complex &point, const int iterations) {
    Complex z = point;
    int i;
    for (i = 0; i < iterations; ++i) {
        z = z * z + Fractal::Julia::juliaConstant;
        if (z.norm() > 2.0)
            break;
    }
    return i;
}

/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans l'ensemble de Mandelbrot
 * @param point
 * @param iterations
 * @return int
 */
int Fractal::Mandelbrot::pointCheck(const Complex &point, const int iterations) {
    int i;
    Complex z = Complex(0, 0);
    for (i = 0; i < iterations; ++i) {
        z = z * z + point;
        if (z.norm() > 2.0)
            break;
    }
    return i;
}


/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans l'ensemble de BurningShip
 * @param point
 * @param iterations
 * @return
 */
int Fractal::BurningShip::pointCheck(const Complex &point, const int iterations) {
    int i;
    Complex z = Complex(0, 0);
    for (i = 0; i < iterations; ++i) {
        z.real = abs(z.real);
        z.imag = abs(z.imag);
        z = z * z + point;
        if (z.norm() > 2.0)
            break;
    }
    return i;
}

/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans l'ensemble de Newton
 * @param point
 * @param iterations
 * @return
 */
int Fractal::Newton::pointCheck(const Complex &point, const int iterations) {
    Complex z = point;
    for (int i = 0; i < iterations; ++i) {
        Complex f_z = z * z * z - Complex(1.0, 0.0);

        Complex f_prime = Complex(3.0, 0.0) * z * z;

        z = z - f_z / f_prime;

        if (f_z.norm() < 1e-6) {
            return i;
        }
    }
    return iterations;
}

/**
 * @brief Fonction pour calculer le nombre d'itérations pour un point donné dans l'ensemble de BarnsleyFern
 * @param point
 * @param iterations
 * @return
 */
int Fractal::Koch::pointCheck(const Fractalium::Complex &point, const int iterations) {
    Complex z = point;
    for (int i = 0; i < iterations; ++i) {
        Complex f_z = z * z * z * z - Complex(1.0, 0.0);
        Complex f_prime = Complex(4.0, 0.0) * z * z * z;
        z = z - f_z / f_prime;
        if (f_z.norm() < 1e-6) {
            return i;
        }
    }
    return iterations;
}