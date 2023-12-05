#include <QApplication>
#include <mainwindow.hpp>
#include <iostream>
#include <MPI.hpp>
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;
namespace frac = Fractalium;

int main(int argc, char *argv[])
{

    mpi::environment env;
    mpi::communicator world;

    frac::MPICalculator mpiCalculator(world.rank());

    frac::MPICalculator::run();


    if (world.rank() == 0)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return QApplication::exec();
    }
}
