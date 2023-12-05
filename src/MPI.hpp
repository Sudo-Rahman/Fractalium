//
// Created by sr-71 on 03/12/2023.
//

#ifndef FRACTALIUM_MPI_HPP
#define FRACTALIUM_MPI_HPP

#include <iostream>
#include <vector>
#include <Double.hpp>
#include <Fractal.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <Image.hpp>
#include <boost/signals2.hpp>


namespace Fractalium
{

    using Fractalium::Double;


    class MPIStruct {

    public:

        MPIStruct(const MPIStruct &aStruct)
        {
            start_end_x = aStruct.start_end_x;
            start_end_y = aStruct.start_end_y;
            iterations = aStruct.iterations;
            step_coord = aStruct.step_coord;
            offset = aStruct.offset;
            fractal = aStruct.fractal;
            image = aStruct.image;
        }

        std::pair<uint16_t, uint16_t> start_end_x;
        std::pair<uint16_t, uint16_t> start_end_y;

        int iterations{};

        Double step_coord;

        std::pair<Double, Double> offset;

        Fractal fractal;

        Image image;

        MPIStruct(const std::pair<uint16_t, uint16_t> &start_end_x,
                  const std::pair<uint16_t, uint16_t> &start_end_y,
                  uint32_t iterations,
                  Double step_coord,
                  std::pair<Double, Double> offset,
                  const Fractalium::Fractal &fractal) :
                start_end_x(start_end_x),
                start_end_y(start_end_y),
                iterations(iterations),
                step_coord(step_coord),
                offset(offset),
                fractal(fractal)
        {
            image = Image(start_end_x.second - start_end_x.first, start_end_y.second - start_end_y.first);
        }

        MPIStruct() = default;


    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & start_end_x;
            ar & start_end_y;
            ar & iterations;
            ar & step_coord;
            ar & offset;
            ar & fractal;
            ar & image;
        }

    };

    class MPICalculator {

    public:
        static boost::signals2::signal<void()> signal;

        static MPIStruct mpi_struct;

        static uint16_t rank;

        static std::atomic<uint16_t> thread_finished;

        static boost::thread_group thread_group_task;

        static boost::thread_group thread_group_io;

        static boost::asio::io_context io_context;
        static boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;

        explicit MPICalculator(uint16_t rank);

        static void calculate(const MPIStruct &mpi_struct);


        static void threadFunction(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y);

        static void send(const MPIStruct &mpi_struct);

        static void receive();

    public:

        static void run();
    };

}


#endif //FRACTALIUM_MPI_HPP
