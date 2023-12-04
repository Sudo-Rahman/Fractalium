//
// Created by sr-71 on 03/12/2023.
//
#include "MPI.hpp"
#include <boost/mpi.hpp>


namespace mpi = boost::mpi;


Fraclium::MPICalculator::MPICalculator(int rank)
{
    _rank = rank;
}

void Fraclium::MPICalculator::run()
{
    _thread_group.create_thread([this]
                                { return _io_context.run(); });
}


void Fraclium::MPICalculator::calculate(MPIStruct mpi_struct)
{
    boost::asio::post(
            [this, mpi_struct]
            {
                this->_mpi_struct = mpi_struct;
                uint8_t thread_count = boost::thread::hardware_concurrency();

                int start_x, end_x, range_x;
                for (int i = 0; i < thread_count; ++i)
                {
                    range_x = mpi_struct.start_end_x.first + mpi_struct.start_end_x.second;
                    start_x = range_x / thread_count * i;
                    end_x = range_x / thread_count * (i + 1);
                    _thread_group.create_thread(
                            [mpi_struct, start_x, end_x]
                            {

                            }
                    );
                }

            });
}

void Fraclium::MPICalculator::threadFunction(int start_x, int end_x, int start_y, int end_y)
{
    for (int x = start_x; x < end_x; ++x)
        for (int y = start_y; y < end_y; ++y)
        {
            Fractalium::Complex point = Fractalium::Complex(
                    Fractalium::Fractal::_offset.first + x * _mpi_struct.step_coord,
                    Fractalium::Fractal::_offset.second + y * _mpi_struct.step_coord
            );
            int q = _mpi_struct.fractal.pointCheck(point, _mpi_struct.iterations);
        }
}


void Fraclium::MPICalculator::receive()
{

}
