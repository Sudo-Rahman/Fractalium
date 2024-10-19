//
// Created by sr-71 on 03/12/2023.
//
#include "MPI.hpp"
#include <boost/mpi.hpp>
#include <Settings.hpp>

namespace mpi = boost::mpi;
namespace fr = Fractalium;
using std::cout;
using std::endl;
Fractalium::MPIStruct Fractalium::MPICalculator::mpi_struct;
uint16_t Fractalium::MPICalculator::rank = 0;
boost::signals2::signal<void()> Fractalium::MPICalculator::finshed;
boost::signals2::signal<void(uint32_t)> Fractalium::MPICalculator::node_recived;
std::future<void> Fractalium::MPICalculator::future;
bool Fractalium::MPICalculator::is_running = false;
uint32_t Fractalium::MPICalculator::node_count = 0;
uint32_t Fractalium::MPICalculator::node_working = 0;

/**
 * @brief Constructeur de la classe MPICalculator
 * @param rank Rang du processus MPI
 */
Fractalium::MPICalculator::MPICalculator(uint16_t rank) {
    MPICalculator::rank = rank;
    node_count = mpi::communicator().size();
}

/**
 * @brief Calcul de l'image fractale en fonction des données reçues par le processus MPI et envoie de l'image calculée au processus 0 (master)
 * @param data Données reçues par le processus MPI
 * @param image Image fractale
 */
void Fractalium::MPICalculator::calculate(const MPIStruct &data, Image &image) {
    auto mpi_data = data;
    if (data.end_x < data.start_x) {
        for (uint16_t x = data.start_x; x < data.width; ++x) {
            for (uint16_t y = data.start_y; y < data.end_y; ++y) {
                Fractalium::Complex point = Fractalium::Complex(
                        mpi_data.offset_x + x * mpi_data.step_coord,
                        mpi_data.offset_y + y * mpi_data.step_coord
                );
                int q = mpi_data.fractal.pointCheck(point, mpi_data.iterations);
                image.setPixel(x, y, q);
            }
        }
        mpi_data.start_x = 0;
        auto delta_y = data.end_y - data.start_y;
        mpi_data.start_y = data.end_y;
        mpi_data.end_y = (data.end_y + delta_y) > data.height ? data.height : data.end_y + delta_y;
    }
    for (uint16_t x = mpi_data.start_x; x < mpi_data.end_x; ++x) {
        for (uint16_t y = mpi_data.start_y; y < mpi_data.end_y; ++y) {
            Fractalium::Complex point = Fractalium::Complex(
                    mpi_data.offset_x + x * mpi_data.step_coord,
                    mpi_data.offset_y + y * mpi_data.step_coord
            );
            int q = mpi_data.fractal.pointCheck(point, mpi_data.iterations);
            image.setPixel(x, y, q);
        }
    }
}

/**
 * @brief Réception des données envoyées par le processus 0 (master) et calcul de l'image fractale
 * @param image
 */
void Fractalium::MPICalculator::receive(Image &image) {
    mpi::communicator world;
    if (rank == 0) {
        auto counter = new std::atomic<uint32_t>(0);
        for (int proc = 1; proc < node_count; ++proc) {
            auto image_tmp = Image();
            world.recv(proc, 1, image_tmp);
            image.merge(image_tmp); // fusion des images fractales calculées par les processus MPI enfants
            (*counter)++;
            node_recived(
                    *counter); // envoie d'un signal pour indiquer qu'un processus MPI enfant a terminé le calcul de l'image fractale

            // si tous les processus MPI enfants ont terminé le calcul de l'image fractale
            if (*counter == node_working) {
                is_running = false;
                finshed(); // envoie d'un signal pour indiquer que le calcul de l'image fractale est terminé
                counter->store(0);
            }
        }
        delete counter;
    }
}

/**
 * @brief master envoie les zone de l'image à traiter aux noeuds enfants
 * @param data Données à envoyer
 * @param image Image fractale
 */
void Fractalium::MPICalculator::send(const MPIStruct &data, Image &image) {
    mpi::communicator world;
    if (rank == 0) {
        auto node = world.size() - 1;
        MPIStruct mpi_tmp = data;
        auto img_pixels = data.width * data.height;
        auto nb_pixel_per_node = img_pixels / (node - 1);
        auto square = [&nb_pixel_per_node] { return sqrt(nb_pixel_per_node); };
        auto x_delta = data.end_x - data.start_x;

        // algorithme de calcul de zone par collumne
        auto collumns = [&](int proc) {
            mpi_tmp.start_x = x_delta / node * proc;
            mpi_tmp.end_x = x_delta / node * (proc + 1);

            world.send(proc + 1, 0, mpi_tmp);
            is_running = true;
        };

        // algorithme de calcul de zone par carré
        auto squares = [&](int proc) {
            mpi_tmp.start_x = uint16_t(proc * square()) % data.end_x;
            mpi_tmp.end_x = uint16_t((proc + 1) * square()) % data.end_x;

            mpi_tmp.start_y = uint16_t((proc * square()) / data.end_x) * square();
            mpi_tmp.end_y = uint16_t(((proc * square()) / data.end_x) + 1) * square();
            if (mpi_tmp.end_y > data.height) mpi_tmp.end_y = data.height;

            world.send(proc + 1, 0, mpi_tmp);
            is_running = true;
        };


        node_working = node;

        switch (Settings::AREA_ALGORITHM_TYPE) {
            default: {
            }
            case Settings::COLLUMNS : {
                auto counter = node;
                while (counter > data.width) counter--;
                for (int proc = 0; proc < counter; ++proc) collumns(proc);
            }
                break;
            case Settings::SQUARES : {
                for (int proc = 0; proc < node; ++proc) squares(proc);
            }
                break;
        }

        future = std::async(std::launch::async, [&] {
            receive(image);
        });
    }
}

/**
 * @brief Ferme le programme et arrete tous les noeuds
 */
void Fractalium::MPICalculator::stop() {
    mpi::communicator().abort(0);
}