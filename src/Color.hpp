//
// Created by sr-71 on 19/12/2023.
//

#ifndef FRACTALIUM_COLOR_HPP
#define FRACTALIUM_COLOR_HPP

#include <boost/serialization/serialization.hpp>


namespace Fractalium {
    struct Color {
        int r, g, b;
    };

    template<typename Archive>
    void serialize(Archive &ar, Color &color, const unsigned int version) {
        ar & color.r;
        ar & color.g;
        ar & color.b;
    }
}

#endif //FRACTALIUM_COLOR_HPP
