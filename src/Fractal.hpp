//
// Created by sr-71 on 28/11/2023.
//

#ifndef FRACTALIUM_FRACTAL_HPP
#define FRACTALIUM_FRACTAL_HPP

#include <Complex.hpp>
#include <iostream>
#include <boost/serialization/serialization.hpp>


namespace Fractalium {


    class Fractal {


    public:
        enum class FractalType {
            Mandelbrot,
            Julia,
            BurningShip,
            Newton,
            Koch
        };

        explicit Fractal(FractalType type = FractalType::Mandelbrot) : _type(type) {}

        [[nodiscard]] int pointCheck(const Complex &pointm, int iterations) const;

        static uint64_t ITERATIONS;

        void setType(FractalType type) {
            _type = type;
        }

        [[nodiscard]] FractalType getType() const {
            return _type;
        }

    private:

        FractalType _type;


        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & _type;
        }

        class Mandelbrot {
        public:
            [[nodiscard]] static int pointCheck(const Complex &point, int iterations);
        };

        class Julia {
            static Complex juliaConstant;
        public:
            [[nodiscard]] static int pointCheck(const Complex &point, int iterations);
        };

        class BurningShip {
        public:
            [[nodiscard]] static int pointCheck(const Complex &point, int iterations);
        };

        class Newton {
        public:
            [[nodiscard]] static int pointCheck(const Complex &point, int iterations);
        };

        class Koch {
        public:
            [[nodiscard]] static int pointCheck(const Complex &point, int iterations);
        };

    };
}

#endif //FRACTALIUM_FRACTAL_HPP
