#include <QApplication>
#include <mainwindow.hpp>
#include <iostream>
#include <MPI.hpp>
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;
namespace frac = Fractalium;

int main(int argc, char *argv[]) {

    mpi::environment env(argc, argv); // MPI init
    mpi::communicator world; // MPI world communicator pour les processus MPI
    frac::MPICalculator mpiCalculator(world.rank());// Création d'un objet MPICalculator pour le processus MPI courant

    // Si le processus MPI courant est le processus 0 (master)
    if (world.rank() == 0) {
        QApplication a(argc, argv); // Qt init
        MainWindow w;// Création d'une fenêtre principale
        w.show(); // Affichage de la fenêtre principale
        return QApplication::exec();
    } else { // Si le processus MPI courant est un processus esclave
        while (true) { // Boucle infinie pour que le processus MPI esclave reste actif et puisse recevoir des données du processus 0 (master) à tout moment
            frac::MPIStruct mpiStruct;
            {
                world.recv(0, 0, mpiStruct);
                std::cout << "received: " << world.rank() << std::endl;
            }
            Fractalium::MPICalculator::mpi_struct = mpiStruct; // Enregistrement des données reçues dans l'objet MPICalculator
            auto image = Fractalium::Image(mpiStruct.width, mpiStruct.height); // Création d'une image fractale
            frac::MPICalculator::calculate(mpiStruct, image); // Calcul de l'image fractale
            {
                world.send(0, 1, image); // Envoi de l'image fractale calculée au processus 0 (master)
            }
        }
    }
}
