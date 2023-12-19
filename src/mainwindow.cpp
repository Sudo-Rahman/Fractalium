#include <QRubberBand>
#include "mainwindow.hpp"
#include <iostream>
#include <chrono>
#include <QApplication>
#include <IterationDialog.hpp>
#include <MPI.hpp>
#include <QFileDialog>
#include <ResizeDialog.hpp>

using std::min;
using std::max;
using Fractalium::Double;

unsigned short color_mode = 0;


struct color {
    int r, g, b;
};

auto get_color = [](const int &i) -> color {
    color c{};

    const double iteration = static_cast<double>(i) / MainWindow::TOTAL_COLORS;
    const double hue = 45.0 + 315.0 * iteration;
    const double C = 1.0 - std::abs(2.0 * iteration - 1.0);
    const double X = C * (1.0 - std::abs(std::fmod(hue / 60.0, 2.0) - 1.0));
    const double m = 0.0;

    switch (color_mode) {

        case 1:
            c.r = static_cast<int>((C + m) * 255);
            c.g = static_cast<int>((X + m) * 255);
            c.b = static_cast<int>(m * 255);
            break;

        case 2:
            c.r = static_cast<int>(m * 255);
            c.g = static_cast<int>((C * 0.5 + m) * 255);
            c.b = static_cast<int>(m * 255);
            break;

        case 3:
            c.r = static_cast<int>(m * 255);
            c.g = static_cast<int>((C + m) * 255);
            c.b = static_cast<int>((X + m) * 255);
            break;

        case 4:
            c.r = static_cast<int>(m * 255);
            c.g = static_cast<int>((X + m) * 255);
            c.b = static_cast<int>((C + m) * 255);
            break;

        case 5:
            c.r = static_cast<int>((X + m) * 255);
            c.g = static_cast<int>(m * 255);
            c.b = static_cast<int>((C + m) * 255);
            break;

        case 6:
            c.r = static_cast<int>((C + m) * 255);
            c.g = static_cast<int>(m * 255);
            c.b = static_cast<int>((X + m) * 255);
            break;

        case 7:
            c.r = static_cast<int>((C + m) * 255);
            c.g = static_cast<int>((C + m) * 255);
            c.b = static_cast<int>(m * 255);
            break;
    }

    return c;
};

color c{};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto *central = new QWidget(this);
    this->setCentralWidget(central);


    _label = new Fractalium::FractalWidget(central);
    _label->setAlignment(Qt::AlignCenter);
    _label->setFixedSize(DISPLAY_SIZE_WIDTH, DISPLAY_SIZE_HEIGHT);

    this->setFixedSize(DISPLAY_SIZE_WIDTH, DISPLAY_SIZE_HEIGHT);

    _color_map = std::vector<QColor>(MainWindow::TOTAL_COLORS);

    for (int i = 0; i < MainWindow::TOTAL_COLORS; i++) {
        c = get_color(i);
        _color_map[i] = QColor::fromRgb(c.r, c.g, c.b).name();
    }

    _color_map.emplace_back(QColor::fromRgb(0, 0, 0).name());
    _image = new QImage(_label->width(), _label->height(), QImage::Format_RGB32);
    _divergence_image = Fractalium::Image(_label->width(), _label->height());

    setupUi();

    _fractal = new Fractalium::Fractal();
    connect(_label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);
    Fractalium::MPICalculator::signal.connect([this] {
        qApp->postEvent(this, new PaintFractalEvent);
    });
}


/**
 * @brief Met à jour la couleur du fractal quand l'utilisateur change de couleur
 */
void MainWindow::updateColor() {
    for (int i = 0; i < MainWindow::TOTAL_COLORS; i++) {
        c = get_color(i);
        _color_map[i] = QColor::fromRgb(c.r, c.g, c.b).name();
    }
}

/**
 * @brief Retourne la couleur en fonction de la divergence et de la couleur max du fractal (couleur dynamique)
 * @param divergence
 * @param maxDivergence
 * @param red
 * @param green
 * @param blue
 * @return
 */
color getColorForDivergence(int divergence, int maxDivergence, int red, int green, int blue) {
    if (divergence == maxDivergence) {
        return {0, 0, 0};
    } else {
        const double factor = (divergence + 1) / 3.0;
        const int r = std::clamp(static_cast<int>(std::lround(factor * red)), 0, 255);
        const int g = std::clamp(static_cast<int>(std::lround(factor * green)), 0, 255);
        const int b = std::clamp(static_cast<int>(std::lround(factor * blue)), 0, 255);
        return {r, g, b};
    }
}


/**
 * @brief Peint le fractal dans l'image
 */
void MainWindow::paintFractal() {
    uint16_t start_x, end_x, end_y, start_y;
    start_x = 0;
    start_y = 0;
    end_x = _divergence_image.width();
    end_y = _divergence_image.height();
    if (color_mode == 0) { // Dynamique color mode (couleur en fonction de la divergence)
        int maxDivergence = 0;
        for (uint16_t x = start_x; x < end_x; ++x) {
            for (uint16_t y = start_y; y < end_y; ++y) {
                if (_divergence_image.getPixel(x, y) > maxDivergence) {
                    maxDivergence = _divergence_image.getPixel(x, y);
                }
            }
        }
        color color{};
        srand(time(nullptr));
        const int red = rand() % 10 + 1;
        const int green = rand() % 10 + 1;
        const int blue = rand() % 10 + 1;

        for (uint16_t x = start_x; x < end_x; ++x) {
            for (uint16_t y = start_y; y < end_y; ++y) {
                if (_divergence_image.getPixel(x, y) == -1) continue;
                color = getColorForDivergence(_divergence_image.getPixel(x, y), maxDivergence, red, green, blue);
                _image->setPixelColor(x, y, QColor::fromRgb(color.r, color.g, color.b));
            }
        }

    } else { // color_mode != 0 donc on utilise la palette de couleur
        for (uint16_t x = start_x; x < end_x; ++x) {
            for (uint16_t y = start_y; y < end_y; ++y) {
                if (_divergence_image.getPixel(x, y) == -1) continue;
                _image->setPixelColor(x, y,
                                      _color_map[min(_divergence_image.getPixel(x, y), MainWindow::TOTAL_COLORS)]);
            }
        }
    }
    _back_history.emplace_back(history{*_image, _offset, _step_coord});
    _label->setFractal(*_image);
}

/**
 * @brief Met à jour le fractal en fonction de la nouvelle selection de l'utilisateur
 * @param start
 * @param end
 */
void MainWindow::newSelection(const QPoint &start, const QPoint &end) {

    // on calcule le nouvel offset x
    _offset.first =
            _offset.first + start.x() * _step_coord;
    // on calcule le nouvel offset y
    _offset.second =
            _offset.second + start.y() * _step_coord + (_step_coord * _label->height() / _label->width());

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

    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _divergence_image);
}

/**
 * @brief Initialise la barre de menu
 */
void MainWindow::setupUi() {

    _menu_bar = new QMenuBar(this);
    this->setMenuBar(_menu_bar);

    auto *menu = new QMenu("Fractal", _menu_bar);
    _menu_bar->addMenu(menu);

    auto *action = new QAction("Mandelbrot", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        _offset = {-2.1, -2};
        _step_coord = max(3.5 / _label->width(), 3.5 / _label->height());
        *_fractal = Fractalium::Fractal();
        mpiCalculate();
    });

    action = new QAction("Julia", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        _offset = {-2.1, -2};
        _step_coord = max(3.5 / _label->width(), 3.5 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Julia);
        mpiCalculate();
    });

    action = new QAction("BurningShip", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        _offset = {-2.2, -2};
        _step_coord = max(3.2 / _label->width(), 3.2 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::BurningShip);
        mpiCalculate();
    });

    action = new QAction("Newton", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        _offset = {-3, -3};
        _step_coord = max(6.0 / _label->width(), 6.0 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Newton);
        mpiCalculate();
    });


    action = new QAction("Koch", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        _offset = {-5, -5};
        _step_coord = max(10.0 / _label->width(), 10.0 / _label->height());
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

    auto menu3 = new QMenu("Couleur", _menu_bar);
    _menu_bar->addMenu(menu3);

    action = new QAction("Dynamique", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 0;
    });

    action = new QAction("Feu", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 1;
        updateColor();
    });

    action = new QAction("Vert", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 2;
        updateColor();
    });

    action = new QAction("Vert clair", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 3;
        updateColor();
    });

    action = new QAction("Bleu", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 4;
        updateColor();
    });

    action = new QAction("Violet", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 5;
        updateColor();
    });

    action = new QAction("Rose", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]() {
        color_mode = 6;
        updateColor();
    });

    auto menu4 = new QMenu("Options", _menu_bar);
    _menu_bar->addMenu(menu4);

    auto resize = new QAction("Resize");
    connect(resize, &QAction::triggered, this, [this]() {
        auto dialog = ResizeDialog({this->size()}, this);
        dialog.exec();
        if (dialog.returnType() == ResizeDialog::Return::OK) {
            auto size = dialog.returnSize();
            this->setFixedSize(size);
            this->_label->setFixedSize(size);
            delete _image;
            _image = new QImage(size, QImage::Format_RGB32);
            _divergence_image = Fractalium::Image(size.width(), size.height());
        }
    });
    menu4->addAction(resize);

    auto iterations = new QAction("Iteration");
    connect(iterations, &QAction::triggered, this, [this]() {
        auto dialog = IterationDialog(Fractalium::Fractal::ITERATIONS, this);
        dialog.exec();
        if (dialog.returnType() == IterationDialog::Return::OK) {
            Fractalium::Fractal::ITERATIONS = dialog.iteration();
        }
    });
    menu4->addAction(iterations);
}

/**
 * @brief Gère les évènements de la fenêtre
 * @param event
 * @return
 */
bool MainWindow::event(QEvent *event) {
    if (event->type() == PaintFractalEvent::PaintFractalEventType) {
        paintFractal();
        return true;
    }
    if (event->type() == QEvent::Close) {
        Fractalium::MPICalculator::stop();
    }
    return QMainWindow::event(event);
}

/**
 * @brief Calcule le fractal en utilisant MPI
 */
void MainWindow::mpiCalculate() {
    Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
            0, _label->width(),
            0, _label->height(),
            _offset.first,
            _offset.second,
            _label->width(), _label->height(),
            static_cast<int>(Fractalium::Fractal::ITERATIONS),
            _step_coord,
            *_fractal
    );
    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _divergence_image);
}

/**
 * @brief Retourne en arrière dans l'historique du fractal

 */
void MainWindow::back() {
    if (_back_history.size() <= 1)
        return;
    _front_history.emplace_back(history{*_image, _offset, _step_coord});
    _back_history.pop_back();
    auto h = _back_history.back();
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image;
    _label->setFractal(*_image);
}

/**
 * @brief Retourne en avant dans l'historique
 */
void MainWindow::front() {
    if (_front_history.empty())
        return;
    auto h = _front_history.back();
    _front_history.pop_back();
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image;
    _back_history.emplace_back(history{*_image, _offset, _step_coord});
    _label->setFractal(*_image);
}

/**
 * @brief Sauvegarde l'image
 */
void MainWindow::saveImage() {
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                 QDir::homePath() + "/fractal.png",
                                                 tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.isEmpty()) return;
    _image->save(fileName);
}