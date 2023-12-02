#include <QRubberBand>
#include "mainwindow.hpp"
#include <iostream>

using std::min;
using std::max;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    this->setCentralWidget(central);


    label = new Fractalium::FractalWidget(central);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumSize(2 * DISPLAY_SIZE + 1, 2 * DISPLAY_SIZE + 1);
    label->setMaximumSize(2 * DISPLAY_SIZE + 1, 2 * DISPLAY_SIZE + 1);

    this->setMinimumSize(2 * DISPLAY_SIZE + 1, 2 * DISPLAY_SIZE + 1);
    this->setMaximumSize(2 * DISPLAY_SIZE + 1, 2 * DISPLAY_SIZE + 1);


    _color_map.resize(Fractalium::Fractal::ITERATIONS);
    _color_map[Fractalium::Fractal::ITERATIONS - 1] = QColor::fromHsl(0, 0, 0).name();
    for (int i = 0, j = 0; j < Fractalium::Fractal::ITERATIONS - 1; i++, ++j)
        _color_map[j] = (i <= DISPLAY_SIZE) ? QColor::fromHsl(i, 200, 148).name() : _color_map[j - 1];

    image = new QImage(label->width(), label->height(), QImage::Format_RGB32);

    setupUi();

    connect(label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);
}


void MainWindow::paintFractal()
{
    for (int i = -label->width() / 2; i <= label->width() / 2; ++i)
    {
        for (int j = -label->height() / 2; j <= label->height() / 2; ++j)
        {
            Fractalium::Complex point = Fractalium::Complex(
                    (i + label->width() / 2) * _step_coord + Fractalium::Fractal::_left_corner.first,
                    (label->height() / 2 - j) * _step_coord + Fractalium::Fractal::_left_corner.second);
            int q = fractal->pointCheck(point, Fractalium::Fractal::ITERATIONS);
            image->setPixelColor(i + label->width() / 2, label->height() / 2 - j, _color_map[q].toRgb());
        }
    }
    label->setFractal(*image);
}

void MainWindow::newSelection(const QPoint &start, const QPoint &end)
{

    Fractalium::Fractal::_left_corner.first =
            min(end.x(), start.x()) * _step_coord + Fractalium::Fractal::_left_corner.first;
    Fractalium::Fractal::_left_corner.second =
            (max(label->width(), label->height()) -
            max(end.y(),start.y())) * _step_coord + Fractalium::Fractal::_left_corner.second;

    std::cout << Fractalium::Fractal::_left_corner.first << "  " << Fractalium::Fractal::_left_corner.second << "  " <<_step_coord
              << std::endl;

    int xDelta = end.x() - start.x(), yDelta = abs(end.y() - start.y());
    _step_coord = double(max(xDelta, yDelta)) / max(label->width(), label->height()) * _step_coord;

    std::cout << _step_coord << std::endl;

    paintFractal();
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
        fractal = new Fractalium::Mandelbrot();
        paintFractal();
    });

    action = new QAction("Julia", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        fractal = new Fractalium::Julia();
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
