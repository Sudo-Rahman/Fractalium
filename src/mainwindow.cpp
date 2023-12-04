#include <QRubberBand>
#include "mainwindow.hpp"
#include <iostream>
#include <chrono>
#include <QApplication>
#include <MPI.hpp>

using std::min;
using std::max;


struct color {
    int r, g, b;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    this->setCentralWidget(central);


    label = new Fractalium::FractalWidget(central);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumSize(DISPLAY_SIZE, DISPLAY_SIZE);
    label->setMaximumSize(DISPLAY_SIZE, DISPLAY_SIZE);

    this->setMinimumSize(DISPLAY_SIZE, DISPLAY_SIZE);
    this->setMaximumSize(DISPLAY_SIZE, DISPLAY_SIZE);

    _step_coord = 4.0 / label->width();


    auto get_color = [](const int &i) -> color
    {
        color c{};
        c.r = i;
        c.g = 200;
        c.b = 229;
        return c;
    };

    _color_map = std::vector<QColor>(Fractalium::Fractal::ITERATIONS);

    color c{};
    for (int i = 0; i < Fractalium::Fractal::ITERATIONS; i++)
    {
        c = get_color((i * 5) % 255);
        _color_map[i] = QColor::fromRgb(c.r, c.g, c.b).name();
    }
    _color_map.emplace_back(QColor::fromRgb(0, 0, 0).name());
    image = new QImage(label->width(), label->height(), QImage::Format_RGB32);

    setupUi();

    connect(label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);

    Fractalium::MPICalculator::signal.connect([this] {
        qApp->postEvent(this, new PaintFractalEvent);
    });
}


//void MainWindow::paintFractal()
//{
//    int q;
//    for (int i = 0; i < label->width(); ++i)
//    {
//        for (int j = 0; j < label->height(); ++j)
//        {
//            Fractalium::Complex point = Fractalium::Complex(
//                    Fractalium::Fractal::_offset.first + i * _step_coord,
//                    Fractalium::Fractal::_offset.second + j * _step_coord
//            );
//            q = fractal->pointCheck(point, Fractalium::Fractal::ITERATIONS);
//            image->setPixelColor(i, j, _color_map[q]);
//        }
//    }
//    label->setFractal(*image);
//}

void MainWindow::paintFractal()
{
    uint16_t start_x, end_x, end_y, start_y;
    start_x = 0;
    end_x = Fractalium::MPICalculator::mpi_struct.image.width();
    start_y = 0;
    end_y = Fractalium::MPICalculator::mpi_struct.image.height();
    for (uint16_t x = start_x; x < end_x; ++x)
    {
        for (uint16_t y = start_y; y < end_y; ++y)
        {
            int q = Fractalium::MPICalculator::mpi_struct.image.getPixel(x, y);
            image->setPixelColor(x, y, _color_map[q]);
        }
    }
    label->setFractal(*image);
}

/**
 * @brief quand on selectionne une zone de l'image
 * @param start
 * @param end
 */
void MainWindow::newSelection(const QPoint &start, const QPoint &end)
{

    // on calcule le nouveau offset x
    Fractalium::Fractal::_offset.first =
            Fractalium::Fractal::_offset.first + start.x() * _step_coord;
    // on calcule le nouveau offset y
    Fractalium::Fractal::_offset.second =
            Fractalium::Fractal::_offset.second + start.y() * _step_coord;

    std::cout << "start x : " << start.x() << " start y : " << start.y() << " offset x : "
              << Fractalium::Fractal::_offset.first << " offset y : " << Fractalium::Fractal::_offset.second
              << std::endl;

    // on calcule le nouveau step_coord
    int xDelta = end.x() - start.x(), yDelta = abs(end.y() - start.y());
    _step_coord = Fractalium::Double(max(xDelta, yDelta)) / Fractalium::Double(label->height() + label->width()) *
                  _step_coord;


    Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
            std::pair<uint16_t, uint16_t>(0, label->width()),
            std::pair<uint16_t, uint16_t>(0, label->height()),
            Fractalium::Fractal::ITERATIONS,
            _step_coord,
            Fractalium::Fractal::_offset,
            *fractal
    );

    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct);
}

void MainWindow::setupUi()
{

    _menu_bar = new QMenuBar(this);
    this->setMenuBar(_menu_bar);

    auto *menu = new QMenu("Fractal", _menu_bar);
    _menu_bar->addMenu(menu);

    auto *action = new QAction("Mandelbrot", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        fractal = new Fractalium::Mandelbrot;
        Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
                std::pair<uint16_t, uint16_t>(0, label->width()),
                std::pair<uint16_t, uint16_t>(0, label->height()),
                Fractalium::Fractal::ITERATIONS,
                _step_coord,
                Fractalium::Fractal::_offset,
                *fractal
        );

        Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct);
    });

    action = new QAction("Julia", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        fractal = new Fractalium::Julia;
        paintFractal();
    });

    auto *menu2 = new QMenu("History", _menu_bar);
    _menu_bar->addMenu(menu2);

    action = new QAction("Back", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        label->back();
    });
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == PaintFractalEvent::PaintFractalEventType)
    {
        paintFractal();
        return true;
    }
    return QMainWindow::event(event);
}
