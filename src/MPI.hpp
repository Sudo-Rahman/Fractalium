//
// Created by sr-71 on 03/12/2023.
//

#ifndef FRACTALIUM_MPI_HPP
#define FRACTALIUM_MPI_HPP

#include <iostream>
#include <vector>
#include <Double.hpp>
#include <Fractal.hpp>
#include <Image.hpp>
#include <future>
#include <boost/signals2.hpp>


namespace Fractalium
{

    using Fractalium::Double;


    class MPIStruct {

    public:

        MPIStruct(const MPIStruct &aStruct)
        {
            start_x = aStruct.start_x;
            end_x = aStruct.end_x;
            start_y = aStruct.start_y;
            end_y = aStruct.end_y;
            offset_x = aStruct.offset_x;
            offset_y = aStruct.offset_y;
            width = aStruct.width;
            height = aStruct.height;
            iterations = aStruct.iterations;
            step_coord = aStruct.step_coord;
            fractal = aStruct.fractal;
        }

        uint16_t start_x;
        uint16_t end_x;

        uint16_t start_y;
        uint16_t end_y;

        Double offset_x;
        Double offset_y;

        uint16_t width;
        uint16_t height;

        int iterations{};

        Double step_coord;

        Fractal fractal;

        MPIStruct(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y, Double offset_x, Double offset_y,
                  uint16_t width, uint16_t height, int iterations, Double step_coord, Fractal fractal) :
                start_x(start_x), end_x(end_x), start_y(start_y), end_y(end_y), offset_x(offset_x),
                offset_y(offset_y), width(width), height(height), iterations(iterations), step_coord(step_coord),
                fractal(fractal)
        {}


        MPIStruct() = default;


    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & start_x;
            ar & end_x;
            ar & start_y;
            ar & end_y;
            ar & offset_x;
            ar & offset_y;
            ar & width;
            ar & height;
            ar & iterations;
            ar & step_coord;
            ar & fractal;
        }

    };

    class MPICalculator {

        static std::future<void> future;

    public:
        static boost::signals2::signal<void()> finshed;

        static boost::signals2::signal<void(uint32_t)> node_recived;

        static MPIStruct mpi_struct;

        static uint16_t rank;

        static bool is_running;

        static uint32_t node_count;
        static uint32_t node_working;

        explicit MPICalculator(uint16_t rank);

        static void calculate(const MPIStruct &mpi_struct, Image &image);

        static void send(const MPIStruct &mpi_struct, Image &image);

        static void receive(Image &image);


    public:

        static void stop();
    };

}


#endif //FRACTALIUM_MPI_HPP
