//
// Created by sr-71 on 03/12/2023.
//
#include "MPI.hpp"
#include <boost/mpi.hpp>


namespace mpi = boost::mpi;

namespace fr = Fractalium;


boost::asio::io_context Fractalium::MPICalculator::io_context;
boost::thread_group Fractalium::MPICalculator::thread_group;
Fractalium::MPIStruct Fractalium::MPICalculator::mpi_struct;
std::atomic<uint16_t> Fractalium::MPICalculator::thread_finished = 0;
uint16_t Fractalium::MPICalculator::rank = 0;

boost::asio::executor_work_guard<boost::asio::io_context::executor_type>  Fractalium::MPICalculator::work_guard(
        io_context.get_executor());

boost::signals2::signal<void()> Fractalium::MPICalculator::signal;

boost::thread Fractalium::MPICalculator::thread_io;


Fractalium::MPICalculator::MPICalculator(uint16_t rank)
{
    rank = rank;
}

void Fractalium::MPICalculator::run()
{
    thread_io = boost::thread([]
                              { return io_context.run(); });

    boost::asio::post(io_context,
                      []
                      { receive(); });
}


void Fractalium::MPICalculator::calculate()
{

    boost::asio::post(io_context,
                      []
                      {
                          uint16_t thread_count = boost::thread::hardware_concurrency();

                          int start_x, end_x, range_x;
                          for (int i = 0; i < thread_count; ++i)
                          {
                              range_x = mpi_struct.start_end_x.first + mpi_struct.start_end_x.second;
                              start_x = range_x / thread_count * i;
                              end_x = range_x / thread_count * (i + 1);
                              thread_group.create_thread(
                                      [start_x, end_x, thread_count]
                                      {
                                          threadFunction(start_x, end_x, mpi_struct.start_end_y.first,
                                                         mpi_struct.start_end_y.second);

                                          thread_finished++;

                                          if (thread_finished == thread_count)
                                          {
//                                              send(mpi_struct);
                                              signal();
                                          }
                                      }
                              );
                          }
                      });
}

void Fractalium::MPICalculator::threadFunction(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y)
{
    for (uint16_t x = start_x; x < end_x; ++x)
    {
        for (uint16_t y = start_y; y < end_y; ++y)
        {
            boost::this_thread::interruption_point();
            Fractalium::Complex point = Fractalium::Complex(
                    mpi_struct.offset.first + x * mpi_struct.step_coord,
                    mpi_struct.offset.second + y * mpi_struct.step_coord
            );
            int q = mpi_struct.fractal.pointCheck(point, mpi_struct.iterations);
            mpi_struct.image.setPixel(x, y, q);
        }
    }
}


void Fractalium::MPICalculator::receive()
{
    mpi::communicator world;

    if (rank == 0)
    {
        thread_group.create_thread(
                [world]
                {
                    while (true)
                    {
                        for (int proc = 1; proc < world.size(); ++proc)
                        {
                            auto mpi_tmp = MPIStruct();
                            world.recv(proc, 0, mpi_tmp);
                            mpi_struct.image.merge(mpi_tmp.image);
                        }
//                        signal();
                    }
                });

    } else
    {
        thread_group.create_thread(
                [world]
                {
                    while (true)
                    {
                        thread_finished = 0;
                        thread_group.interrupt_all();
                        world.recv(0, 0, mpi_struct);
                        calculate();
                    }
                });
    }
}

void Fractalium::MPICalculator::send(const MPIStruct &data)
{
    mpi::communicator world;

    mpi_struct = data;

    thread_finished = 0;

    if (rank == 0)
    {
        for (int proc = 1; proc < world.size(); ++proc)
        {
            world.send(proc, 0, data);
        }
        calculate();
    } else
    {
        world.send(0, 0, data);
    }
}