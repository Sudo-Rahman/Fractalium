#include <QApplication>
#include <mainwindow.hpp>
#include <iostream>
#include <boost/mpi.hpp>

namespace mpi = boost::mpi;

int main(int argc, char *argv[])
{

    mpi::environment env;
    mpi::communicator world;


    if(world.rank() == 0)
    {

        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return QApplication::exec();
    }
    else
    {
        std::cout << "Hello from slave process" << std::endl;
    }
}
