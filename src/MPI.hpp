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


namespace Fraclium
{

    using Fractalium::Double;

    struct MPIStruct {

        std::pair<int, int> start_end_x;
        std::pair<int, int> start_end_y;

        int iterations;

        Double step_coord;

        std::pair<Double, Double> offset;

        Fractalium::Fractal fractal;

        explicit MPIStruct(int iterations, Double step_coord, std::pair<Double, Double> offset) :
                iterations(iterations), step_coord(step_coord), offset(offset)
        {
            start_end_x = {0, 0};
            start_end_y = {0, 0};
        }

        MPIStruct() = default;
    };

    class MPICalculator {

        MPIStruct _mpi_struct{};

        int _rank;

        boost::thread_group _thread_group;

        boost::asio::io_context _io_context{};
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard{_io_context.get_executor()};

        explicit MPICalculator(int rank);

        void calculate(MPIStruct mpi_struct);


        void threadFunction(int start_x, int end_x, int start_y, int end_y);

        void send();

        void receive();

    public:

        void run();
    };

}


#endif //FRACTALIUM_MPI_HPP
