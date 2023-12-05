//
// Created by sr-71 on 04/12/2023.
//

#include "Image.hpp"
#include <iostream>

Fractalium::Image::Image(uint32_t size_x, uint32_t size_y)
{
    _width = size_x;
    _height = size_y;
    _image.resize(size_x);
    for (auto &i: _image)
        i.resize(size_y);

//    init all pixels to -1
    for (auto &i: _image)
        for (auto &j: i)
            j = -1;
}

void Fractalium::Image::setPixel(uint16_t x, uint16_t y, int value)
{
    if (x >= width() || y >= height()){
        std::cout << "x: " << x << " y: " << y << std::endl;
        throw std::out_of_range("Pixel out of range");
    }
    std::lock_guard<std::mutex> lock(mutex);
    _image[x][y] = value;
}

std::ostream &Fractalium::operator<<(std::ostream &os, const Fractalium::Image &image)
{
    for (int i = 0; i < image.width(); ++i)
    {
        for (int j = 0; j < image.height(); ++j)
        {
            os << image._image[i][j] << " ";
        }
        os << std::endl;
    }

    return os;
}

Fractalium::Image::Image(Fractalium::Image &&other) noexcept
{
    _image = std::move(other._image);
    _width = other._width;
    _height = other._height;

}

void Fractalium::Image::merge(Fractalium::Image &image)
{
    if (width() != image.width() || height() != image.height())
        throw std::invalid_argument("Images must have the same size");

    std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < width(); ++i)
    {
        for (int j = 0; j < height(); ++j)
        {
            if (image._image[i][j] != -1)
                _image[i][j] = image._image[i][j];
        }
    }
}

Fractalium::Image &Fractalium::Image::operator=(const Fractalium::Image &other)
{
    if (this != &other)
    {
        _image = other._image;
        _width = other._width;
        _height = other._height;
    }
    return *this;
}



