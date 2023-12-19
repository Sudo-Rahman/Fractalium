//
// Created by sr-71 on 04/12/2023.
//

#ifndef FRACTALIUM_IMAGE_HPP
#define FRACTALIUM_IMAGE_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

namespace Fractalium
{
    class Image {
        std::vector<std::vector<int>> _image;
        uint32_t _width{};
        uint32_t _height{};

    public:

        Image() = default;

        Image(Image &&other) noexcept;

        Image &operator=(const Image &other);

        explicit Image(uint32_t width, uint32_t height);

        void setPixel(uint16_t x, uint16_t y, int value);

        void merge(Image &image);

        int getPixel(int x, int y)
        {
            return _image[x][y];
        }

        [[nodiscard]] uint16_t width() const
        { return _width; }

        [[nodiscard]] uint16_t height() const
        { return _height; }

        friend std::ostream &operator<<(std::ostream &os, const Image &image);

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & _image;
            ar & _width;
            ar & _height;
        }
    };

}


#endif //FRACTALIUM_IMAGE_HPP
