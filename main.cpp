#include <QApplication>
#include <mainwindow.hpp>
#include <iostream>
#include <MPI.hpp>
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;
namespace frac = Fractalium;

int main(int argc, char *argv[])
{

    mpi::environment env(argc, argv);
    mpi::communicator world;

    frac::MPICalculator mpiCalculator(world.rank());


    if (world.rank() == 0)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return QApplication::exec();
    } else
    {
        while (true)
        {
            frac::MPIStruct mpiStruct;
            {
                auto request = world.irecv(0, 0, mpiStruct);
                request.wait();
                std::cout << "received: " << world.rank() << std::endl;
            }
            Fractalium::MPICalculator::mpi_struct = mpiStruct;
            auto image = Fractalium::Image(mpiStruct.width, mpiStruct.height);
            frac::MPICalculator::calculate(mpiStruct, image);
            {
                auto request = world.isend(0, 1, image);
                request.wait();
            }
        }
    }
}
