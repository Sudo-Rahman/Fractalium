#include <QRubberBand>
#include "mainwindow.hpp"
#include <iostream>
#include <chrono>

using std::min;
using std::max;

struct color
{
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
    for (int i = 0;i < Fractalium::Fractal::ITERATIONS ; i++)
    {
        c = get_color((i*5)%255);
        _color_map[i]=QColor::fromRgb(c.r, c.g, c.b).name();
    }
    _color_map.emplace_back(QColor::fromRgb(0, 0, 0).name());
    image = new QImage(label->width(), label->height(), QImage::Format_RGB32);

    setupUi();

    connect(label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);
}


void MainWindow::paintFractal()
{
    int q;
    for (int i = 0; i < label->width(); ++i)
    {
        for (int j = 0; j < label->height(); ++j)
        {
            Fractalium::Complex point = Fractalium::Complex(
                    Fractalium::Fractal::_offset.first + i * _step_coord,
                    Fractalium::Fractal::_offset.second + j * _step_coord
            );
            q = fractal->pointCheck(point, Fractalium::Fractal::ITERATIONS);
            image->setPixelColor(i, j, _color_map[q]);
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

    // on calcule le nouveau step_coord
    int xDelta = end.x() - start.x(), yDelta = abs(end.y() - start.y());
    _step_coord = min(xDelta, yDelta) * _step_coord;

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
        fractal = new Fractalium::Mandelbrot;
        paintFractal();
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
