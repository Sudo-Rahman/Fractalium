//
// Created by sr-71 on 03/12/2023.
//
#include "MPI.hpp"
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;
namespace fr = Fractalium;
using std::cout;
using std::endl;
Fractalium::MPIStruct Fractalium::MPICalculator::mpi_struct;
uint16_t Fractalium::MPICalculator::rank = 0;
boost::signals2::signal<void()> Fractalium::MPICalculator::signal;

/**
 * @brief Constructeur de la classe MPICalculator
 * @param rank Rang du processus MPI
 */
Fractalium::MPICalculator::MPICalculator(uint16_t rank)
{
    MPICalculator::rank = rank;
}

/**
 * @brief Calcul de l'image fractale en fonction des données reçues par le processus MPI et envoie de l'image calculée au processus 0 (master)
 * @param data Données reçues par le processus MPI
 * @param image Image fractale
 */
void Fractalium::MPICalculator::calculate(const MPIStruct &data, Image &image)
{

    for (uint16_t x = data.start_x; x < data.end_x; ++x)
    {
        for (uint16_t y = data.start_y; y < data.end_y; ++y)
        {
            boost::this_thread::interruption_point();
            Fractalium::Complex point = Fractalium::Complex(
                    mpi_struct.offset_x + x * mpi_struct.step_coord,
                    mpi_struct.offset_y + y * mpi_struct.step_coord
            );
            int q = mpi_struct.fractal.pointCheck(point, mpi_struct.iterations);
            image.setPixel(x, y, q);
        }
    }
}

/**
 * @brief Réception des données envoyées par le processus 0 (master) et calcul de l'image fractale
 * @param image
 */
void Fractalium::MPICalculator::receive(Image &image)
{
    mpi::communicator world;
    if (rank == 0)
    {
        auto counter = new std::atomic<uint16_t>(0);
        for (int proc = 1; proc < world.size(); ++proc)
        {
            auto image_tmp = Image();
            world.recv(proc, 1, image_tmp);
            image.merge(image_tmp);
            (*counter)++;
            if (*counter == world.size() - 1)
            {
                signal();
                counter->store(0);
            }
        }
        delete counter;
    }
}

/**
 * @brief master envoie des processus à traiter aux noeuds enfants
 * @param data Données à envoyer
 * @param image Image fractale
 */
void Fractalium::MPICalculator::send(const MPIStruct &data, Image &image)
{
    mpi::communicator world;
    if (rank == 0)
    {
        MPIStruct mpi_tmp = data;
        auto x_delta = mpi_tmp.end_x - mpi_tmp.start_x;
        for (int proc = 0; proc < world.size() - 1; ++proc)
        {
            mpi_tmp.start_x = x_delta / (world.size() - 1) * proc;
            mpi_tmp.end_x = x_delta / (world.size() - 1) * (proc + 1);
            world.send(proc + 1, 0, mpi_tmp);
        }
        receive(image);
    }
}

/**
 * @brief Fermer le programme et arreter tous les noeuds
 */
void Fractalium::MPICalculator::stop()
{
    exit(0);
}