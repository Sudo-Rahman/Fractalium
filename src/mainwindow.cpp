#include <QRubberBand>
#include "mainwindow.hpp"
#include <iostream>
#include <chrono>
#include <QApplication>
#include <MPI.hpp>

using std::min;
using std::max;
using Fractalium::Double;

struct color {
    int r, g, b;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    this->setCentralWidget(central);


    _label = new Fractalium::FractalWidget(central);
    _label->setAlignment(Qt::AlignCenter);
    _label->setMinimumSize(DISPLAY_SIZE, DISPLAY_SIZE);
    _label->setMaximumSize(DISPLAY_SIZE, DISPLAY_SIZE);

    this->setMinimumSize(DISPLAY_SIZE, DISPLAY_SIZE);
    this->setMaximumSize(DISPLAY_SIZE, DISPLAY_SIZE);


    _step_coord = 4.0 / _label->width();


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


    _image = new QImage(_label->width(), _label->height(), QImage::Format_RGB32);

    _diergence_image = Fractalium::Image(_label->width(), _label->height());

    setupUi();

    _fractal = new Fractalium::Fractal();

    connect(_label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);

    Fractalium::MPICalculator::signal.connect([this]
                                              {
                                                  qApp->postEvent(this, new PaintFractalEvent);
                                              });
}


void MainWindow::paintFractal()
{
    uint16_t start_x, end_x, end_y, start_y;
    start_x = 0;
    end_x = _diergence_image.width();
    start_y = 0;
    end_y = _diergence_image.height();

    for (uint16_t x = start_x; x < end_x; ++x)
    {
        for (uint16_t y = start_y; y < end_y; ++y)
        {
            if (_diergence_image.getPixel(x, y) == -1)
                continue;
            int q = _diergence_image.getPixel(x, y);
            _image->setPixelColor(x, y, _color_map[q]);
        }
    }
    _back_history.emplace_back(history{*_image, _offset, _step_coord});
    _label->setFractal(*_image);
}

/**
 * @brief quand on selectionne une zone de l'image
 * @param start
 * @param end
 */
void MainWindow::newSelection(const QPoint &start, const QPoint &end)
{

    // on calcule le nouveau offset x
    _offset.first =
            _offset.first + start.x() * _step_coord;
    // on calcule le nouveau offset y
    _offset.second =
            _offset.second + start.y() * _step_coord;

    // on calcule le nouveau step_coord
    int xDelta = abs(end.x() - start.x()), yDelta = abs(end.y() - start.y());
    _step_coord = Double(max(xDelta, yDelta)) / Double(_label->height() + _label->width()) *
                  _step_coord;


    Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
            0, _label->width(),
            0, _label->height(),
            _offset.first,
            _offset.second,
            _label->width(), _label->height(),
            Fractalium::Fractal::ITERATIONS,
            _step_coord,
            *_fractal
    );

    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _diergence_image);
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
        *_fractal = Fractalium::Fractal();
        mpiCalculate();
    });

    action = new QAction("Julia", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Julia);
        mpiCalculate();
    });

    action = new QAction("BurningShip", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::BurningShip);
        mpiCalculate();
    });

    action = new QAction("Newton", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Newton);
        mpiCalculate();
    });


    action = new QAction("Koch", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Koch);
        mpiCalculate();
    });

    auto *menu2 = new QMenu("History", _menu_bar);
    _menu_bar->addMenu(menu2);

    action = new QAction("Back", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::back);

    action = new QAction("Front", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::front);

    action = new QAction("Save", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::saveImage);

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

void MainWindow::mpiCalculate()
{
    Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
            0, _label->width(),
            0, _label->height(),
            _offset.first,
            _offset.second,
            _label->width(), _label->height(),
            Fractalium::Fractal::ITERATIONS,
            _step_coord,
            *_fractal
    );
    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _diergence_image);
}

void MainWindow::back()
{
    if (_back_history.empty())
        return;
    auto h = _back_history.back();
    _back_history.pop_back();
    _front_history.emplace_back(history{*_image, _offset, _step_coord});
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image;
    _label->setFractal(*_image);
}

void MainWindow::front()
{
    if (_front_history.empty())
        return;
    auto h = _front_history.back();
    _front_history.pop_back();
    _back_history.emplace_back(history{*_image, _offset, _step_coord});
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image;
    _label->setFractal(*_image);
}

void MainWindow::saveImage()
{
    _image->save("fractal.png");
}