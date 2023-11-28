//
// Created by sr-71 on 28/11/2023.
//

#ifndef MANDELBROT_CONTANTES_HPP
#define MANDELBROT_CONTANTES_HPP

#include <cmath>
#include <utility>

const int ITERATIONS = 100;
int DISPLAY_SIZE = 200;

std::pair<double, double> leftCorner = {-2.5, -2};
double STEP_COORD = 2.0 / double(DISPLAY_SIZE);

#endif //MANDELBROT_CONTANTES_HPP
