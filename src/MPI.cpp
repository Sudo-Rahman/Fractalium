//
// Created by sr-71 on 03/12/2023.
//
#include "MPI.hpp"
#include <boost/mpi.hpp>


namespace mpi = boost::mpi;

namespace fr = Fractalium;

using std::cout;
using std::endl;


boost::asio::io_context Fractalium::MPICalculator::io_context;
boost::thread_group Fractalium::MPICalculator::thread_group_io;
boost::thread_group Fractalium::MPICalculator::thread_group_task;
Fractalium::MPIStruct Fractalium::MPICalculator::mpi_struct;
std::atomic<uint16_t> Fractalium::MPICalculator::thread_finished = 0;
uint16_t Fractalium::MPICalculator::rank = 0;

boost::asio::executor_work_guard<boost::asio::io_context::executor_type>  Fractalium::MPICalculator::work_guard(
        io_context.get_executor());

boost::signals2::signal<void()> Fractalium::MPICalculator::signal;


Fractalium::MPICalculator::MPICalculator(uint16_t rank)
{
    MPICalculator::rank = rank;
}

void Fractalium::MPICalculator::run()
{

    thread_group_io.create_thread([]
                                  { io_context.run(); });

    boost::asio::post(io_context, []
    { receive(); });
}


void Fractalium::MPICalculator::calculate(const MPIStruct &data)
{

    boost::asio::post(io_context,
                      [data]
                      {
                          thread_finished = 0;

                          uint16_t thread_count = boost::thread::hardware_concurrency();

                          int start_x, end_x, range_x;
                          for (int i = 0; i < thread_count; ++i)
                          {
                              range_x = data.start_end_x.second - data.start_end_x.first;
                              start_x = range_x / thread_count * i + data.start_end_x.first;
                              end_x = range_x / thread_count * (i + 1) + data.start_end_x.first;

                              thread_group_task.create_thread(
                                      [start_x, end_x, thread_count, data]
                                      {
                                          threadFunction(start_x, end_x, data.start_end_y.first,
                                                         data.start_end_y.second);

                                          thread_finished++;

                                          if(mpi::communicator().size() == 1)
                                          {
                                              if (thread_finished == thread_count)
                                              {
                                                  signal();
                                              }
                                          }
                                          else
                                          {
                                              if (thread_finished == thread_count)
                                              {
                                                  if (rank not_eq 0)
                                                  {
                                                      send(mpi_struct);
                                                  }
                                              }
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
        thread_group_io.create_thread(
                [world]
                {
                    while (true)
                    {
                        for (int proc = 1; proc < world.size(); ++proc)
                        {
                            auto mpi_tmp = MPIStruct();
                            world.recv(proc, 1, mpi_tmp);
                            mpi_struct.image.merge(mpi_tmp.image);
                        }
                    }
                });

    } else
    {
        thread_group_io.create_thread(
                [world]
                {
                    while (true)
                    {
                        world.recv(0, 0, mpi_struct);
//                        thread_group_task.interrupt_all();
                        calculate(mpi_struct);
                    }
                });
    }
}

void Fractalium::MPICalculator::send(const MPIStruct &data)
{
    mpi::communicator world;

    if (rank == 0)
    {
        MPIStruct mpi_tmp = data;

        auto x_delta = mpi_tmp.start_end_x.second - mpi_tmp.start_end_x.first;

        for (int proc = 1; proc < world.size(); ++proc)
        {
            mpi_tmp.start_end_x.first = x_delta / world.size() * proc;
            mpi_tmp.start_end_x.second = x_delta / world.size() * (proc + 1);
            world.send(proc, 1, mpi_tmp);
        }
        mpi_tmp.start_end_x.first = 0;
        mpi_tmp.start_end_x.second = x_delta / world.size();
        thread_group_task.interrupt_all();
        calculate(mpi_tmp);
    } else
    {
        world.send(0, 1, data);
    }
}