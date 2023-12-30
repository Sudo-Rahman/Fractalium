#include "mainwindow.hpp"
#include <iostream>
#include <QApplication>
#include <MPI.hpp>
#include <QFileDialog>
#include <QLayout>
#include <QMessageBox>
#include <Color.hpp>
#include <SnapshotDialog.hpp>
#include <csignal>
#include <History.hpp>
#include <Settings.hpp>
#include <SettingsDialog.hpp>


using std::min;
using std::max;
using Fractalium::Double;
using Fractalium::Color;
using Fractalium::Settings;

unsigned short color_mode = 0;
MainWindow *MainWindow::instance = nullptr;

auto get_color = [](const int &i) -> Color
{
    Color c{};

    const double iteration = static_cast<double>(i) / MainWindow::TOTAL_COLORS;
    const double hue = 45.0 + 315.0 * iteration;
    const double C = 1.0 - std::abs(2.0 * iteration - 1.0);
    const double X = C * (1.0 - std::abs(std::fmod(hue / 60.0, 2.0) - 1.0));
    const double m = 0.0;

    switch (color_mode)
    {

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

Color c{};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    setWindowTitle("Fractalium");

    auto *central = new QWidget(this);
    this->setCentralWidget(central);

    _status_bar = new QStatusBar(this);
    this->setStatusBar(_status_bar);

    if(Fractalium::MPICalculator::node_count <2)
    {
        _status_bar->showMessage("2 noeuds MPI minimum requis");
        return;
    }

    Settings::init();

    _lauched_time = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss");

    instance = this;

    MainWindow::handleSignal();

    _label = new Fractalium::FractalWidget(central);
    _label->setAlignment(Qt::AlignCenter);
    _label->setFixedSize(Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT);


    this->setFixedSize(Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT);

    Fractalium::MPICalculator::node_recived.connect(
            [this](const uint32_t &counter)
            {
                _status_bar->showMessage("Reception des données du noeud " + QString::number(counter) + "/" +
                                         QString::number(Fractalium::MPICalculator::node_count - 1));
            });


    _color_map = std::vector<QColor>(MainWindow::TOTAL_COLORS);

//    initilisation de la palette de couleur
    for (int i = 0; i < MainWindow::TOTAL_COLORS; i++)
    {
        c = get_color(i);
        _color_map[i] = QColor::fromRgb(c.r, c.g, c.b).name();
    }

    _color_map.emplace_back(QColor::fromRgb(0, 0, 0).name());
    _image = new QImage(_label->width(), _label->height(), QImage::Format_RGB32);
    _divergence_image = Fractalium::Image(_label->width(), _label->height());

    setupUi();

    _fractal = new Fractalium::Fractal();
    connect(_label, &Fractalium::FractalWidget::newSelection, this, &MainWindow::newSelection);
    Fractalium::MPICalculator::finshed.connect(
            [this]
            {
                qApp->postEvent(this, new PaintFractalEvent);
                _status_bar->showMessage("Calcul terminé");
                _label->enableSelection();
            });

    // on regarde si le programme a crashé ou non
    lauchAfterCrash();
}


/**
 * @brief Met à jour la couleur du fractal quand l'utilisateur change de couleur
 */
void MainWindow::updateColor()
{
    for (int i = 0; i < MainWindow::TOTAL_COLORS; i++)
    {
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
Color getColorForDivergence(int divergence, int maxDivergence, int red, int green, int blue)
{
    if (divergence == maxDivergence)
    {
        return {0, 0, 0};
    } else
    {
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
void MainWindow::paintFractal()
{
    uint16_t start_x, end_x, end_y, start_y;
    start_x = 0;
    start_y = 0;
    end_x = _divergence_image.width();
    end_y = _divergence_image.height();
    if (color_mode == 0)
    { // Dynamique color mode (couleur en fonction de la divergence)
        int maxDivergence = 0;
        for (uint16_t x = start_x; x < end_x; ++x)
        {
            for (uint16_t y = start_y; y < end_y; ++y)
            {
                if (_divergence_image.getPixel(x, y) > maxDivergence)
                {
                    maxDivergence = _divergence_image.getPixel(x, y);
                }
            }
        }
        Color color{};
        srand(time(nullptr));
        const int red = rand() % 10 + 1;
        const int green = rand() % 10 + 1;
        const int blue = rand() % 10 + 1;

        for (uint16_t x = start_x; x < end_x; ++x)
        {
            for (uint16_t y = start_y; y < end_y; ++y)
            {
                if (_divergence_image.getPixel(x, y) == -1) continue;
                color = getColorForDivergence(_divergence_image.getPixel(x, y), maxDivergence, red, green, blue);
                _image->setPixelColor(x, y, QColor::fromRgb(color.r, color.g, color.b));
            }
        }

    } else
    { // color_mode != 0 donc on utilise la palette de couleur
        for (uint16_t x = start_x; x < end_x; ++x)
        {
            for (uint16_t y = start_y; y < end_y; ++y)
            {
                if (_divergence_image.getPixel(x, y) == -1) continue;
                _image->setPixelColor(x, y,
                                      _color_map[min(_divergence_image.getPixel(x, y), MainWindow::TOTAL_COLORS)]);
            }
        }
    }
    _back_history.emplace_back(Fractalium::History{*_image, _offset, _step_coord});
    _label->setFractal(*_image);
}

/**
 * @brief Met à jour le fractal en fonction de la nouvelle selection de l'utilisateur
 * @param start
 * @param end
 */
void MainWindow::newSelection(const QPoint &start, const QPoint &end)
{

    if (Fractalium::MPICalculator::is_running) return;

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
            static_cast<int>(Settings::ITERATIONS),
            _step_coord,
            *_fractal
    );

    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _divergence_image);
    _status_bar->showMessage("Calcul en cours");
    _label->disableSelection();
}

/**
 * @brief Creation de la barre de menu
 */
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
        _offset = {-2.1, -2};
        _step_coord = max(3.5 / _label->width(), 3.5 / _label->height());
        *_fractal = Fractalium::Fractal();
        mpiCalculate();
    });

    action = new QAction("Julia", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        _offset = {-2.0, -2.0};
        _step_coord = max(3.5 / _label->width(), 3.9 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Julia);
        mpiCalculate();
    });

    action = new QAction("BurningShip", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        _offset = {-2.0, -2.0};
        _step_coord = max(3.0 / _label->width(), 3.0 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::BurningShip);
        mpiCalculate();
    });

    action = new QAction("Newton", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        _offset = {-3, -3};
        _step_coord = max(6.0 / _label->width(), 6.0 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Newton);
        mpiCalculate();
    });


    action = new QAction("Koch", menu);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        _offset = {-5, -5};
        _step_coord = max(10.0 / _label->width(), 10.0 / _label->height());
        *_fractal = Fractalium::Fractal(Fractalium::Fractal::FractalType::Koch);
        mpiCalculate();
    });

    auto *menu2 = new QMenu("Historique", _menu_bar);
    _menu_bar->addMenu(menu2);

    action = new QAction("Arrière", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::back);

    action = new QAction("Avant", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::front);

    action = new QAction("Enregistrer", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::saveImage);

    action = new QAction("Instanté", menu2);
    menu2->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        auto snap = Fractalium::SnapshotHistory
                {
                        _back_history, _front_history,
                        *_fractal
                };
        auto dialog = SnapshotDialog(snap, this);
        dialog.exec();
        if (dialog.returnType() == SnapshotDialog::Return::Import)
        {
            loadSnapshot(snap);
        }
    });

    auto menu3 = new QMenu("Couleur", _menu_bar);
    _menu_bar->addMenu(menu3);

    action = new QAction("Dynamique", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 0;
    });

    action = new QAction("Feu", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 1;
        updateColor();
    });

    action = new QAction("Vert", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 2;
        updateColor();
    });

    action = new QAction("Vert clair", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 3;
        updateColor();
    });

    action = new QAction("Bleu", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 4;
        updateColor();
    });

    action = new QAction("Violet", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 5;
        updateColor();
    });

    action = new QAction("Rose", menu3);
    menu3->addAction(action);
    connect(action, &QAction::triggered, this, [this]()
    {
        color_mode = 6;
        updateColor();
    });

    auto menu4 = new QMenu("Options", _menu_bar);
    _menu_bar->addMenu(menu4);

    auto resize = new QAction("Paramètres");
    connect(resize, &QAction::triggered, this, [this]()
    {
        auto dialog = SettingsDialog(this);
        dialog.exec();
        this->setFixedSize(Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT);
        this->_label->setFixedSize(Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT);
        delete _image;
        _image = new QImage({Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT}, QImage::Format_RGB32);
        _divergence_image = Fractalium::Image(Settings::DISPLAY_SIZE_WIDTH, Settings::DISPLAY_SIZE_HEIGHT);

    });
    menu4->addAction(resize);

}

/**
 * @brief Gère les évènements de la fenêtre
 * @param event
 * @return
 */
bool MainWindow::event(QEvent *event)
{
    if (event->type() == PaintFractalEvent::PaintFractalEventType)
    {
        paintFractal();
        // si l'option est activée, on crée un snapshot
        if (Settings::AUTO_SNAPSHOTS)
        {
            auto snap_name = ("snapshot_" + _lauched_time +
                              ".fractalium").toStdString();
            Fractalium::SnapshotHistory snapshotHistory
                    {
                            _back_history, _front_history,
                            *_fractal
                    };
            Fractalium::makeSnapshot(Settings::SAVE_PATH + "/" + snap_name, snapshotHistory);
            _status_bar->showMessage("Calcul terminé et instantané sauvegardé");
        }
        return true;
    }
    if (event->type() == QEvent::Close)
    {
        Fractalium::MPICalculator::stop();
    }
    return QMainWindow::event(event);
}

/**
 * @brief Calcule le fractal en utilisant MPI
 */
void MainWindow::mpiCalculate()
{
    Fractalium::MPICalculator::mpi_struct = Fractalium::MPIStruct(
            0, _label->width(),
            0, _label->height(),
            _offset.first,
            _offset.second,
            _label->width(), _label->height(),
            static_cast<int>(Settings::ITERATIONS),
            _step_coord,
            *_fractal
    );
    Fractalium::MPICalculator::send(Fractalium::MPICalculator::mpi_struct, _divergence_image);
}

/**
 * @brief Retourne en arrière dans l'historique du fractal

 */
void MainWindow::back()
{
    if (_back_history.size() <= 1)
        return;
    _front_history.emplace_back(Fractalium::History{*_image, _offset, _step_coord});
    _back_history.pop_back();
    auto h = _back_history.back();
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image.toQImage();
    _label->setFractal(*_image);
}

/**
 * @brief Retourne en avant dans l'historique du fractal 
 */
void MainWindow::front()
{
    if (_front_history.empty())
        return;
    auto h = _front_history.back();
    _front_history.pop_back();
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image.toQImage();
    _back_history.emplace_back(Fractalium::History{*_image, _offset, _step_coord});
    _label->setFractal(*_image);
}

/**
 * @brief Sauvegarde l'image dans un fichier 
 */
void MainWindow::saveImage()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer l'image"),
                                                 QDir::homePath() + "/fractal.png",
                                                 tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.isEmpty()) return;
    _image->save(fileName);
}

/**
 * @brief Gère les signaux de la fenêtre (SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGINT, SIGTERM) et sauvegarde l'image dans snapshot.fractalium 
 * @param signum
 * @param backHistory 
 * @param frontHistory 
 * @param fractal 
 */
void MainWindow::signalSnapshot(int signum, const std::vector<Fractalium::History> &backHistory,
                                const std::vector<Fractalium::History> &frontHistory, const Fractalium::Fractal &fractal)
{
    Fractalium::SnapshotHistory snapshotHistory{backHistory, frontHistory, fractal};
    Fractalium::makeSnapshot(Settings::CRASH_SNAP_PATH, snapshotHistory);
    std::cout << "Sauvegarde de l'image dans snapshot.fractalium pour cause de signal [" << signum << "]" << std::endl;
    Settings::setCrash();
    std::exit(signum);
}

/**
 * @brief Gère les signaux de la fenêtre (SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGINT, SIGTERM)
 * SIGSEGV : Segmentation fault
 * SIGABRT : Abort (assertion) ou appel à abort() 
 * SIGFPE : Floating point exception (division par 0) ou erreur de calcul en virgule flottante (NaN)
 * SIGILL : Instruction illégale (instruction inconnue ou instruction interdite) 
 * SIGINT : Interruption (Ctrl+C) 
 * SIGTERM : Demande de terminaison (kill) 
 */
void MainWindow::handleSignal()
{
    if(instance == nullptr) return;

    std::signal(SIGSEGV, [](int signum)
    {
        std::cout << "SIGSEGV" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
    std::signal(SIGABRT, [](int signum)
    {
        std::cout << "SIGABRT" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
    std::signal(SIGFPE, [](int signum)
    {
        std::cout << "SIGFPE" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
    std::signal(SIGILL, [](int signum)
    {
        std::cout << "SIGILL" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
    std::signal(SIGINT, [](int signum)
    {
        std::cout << "SIGINT" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
    std::signal(SIGTERM, [](int signum)
    {
        std::cout << "SIGTERM" << std::endl;
        MainWindow::signalSnapshot(signum, instance->_back_history, instance->_front_history, *instance->_fractal);
    });
}

/**
 * @brief Charge un snapshot
 * @param snapshot
 */
void MainWindow::loadSnapshot(const Fractalium::SnapshotHistory &snapshot)
{
    delete _fractal;
    _fractal = new Fractalium::Fractal(snapshot.fractal);
    _back_history = snapshot.history_back;
    _front_history = snapshot.history_front;
    auto h = _back_history.back();
    _offset = h.offset;
    _step_coord = h.step_coord;
    *_image = h.image.toQImage();
    _label->setFractal(*_image);
}

/**
 * @brief Avertit que le programme a crashé et propose de charger la sauvegarde de l'image
 */
void MainWindow::lauchAfterCrash()
{
    if (Settings::IS_CRASHED)
    {
        auto box = QMessageBox::critical(this, "Crash détecté",
                                         "Le programme a quitte de manière inattendue."
                                         " Voulez-vous charger la sauvegarde de l'image ?",
                                         QMessageBox::Yes | QMessageBox::No);
        if (box == QMessageBox::Yes)
        {
            auto snap = Fractalium::SnapshotHistory{};
            Fractalium::importSnapshot(Settings::CRASH_SNAP_PATH, snap);
            loadSnapshot(snap);
        }
        Settings::resetCrash();
    }
}