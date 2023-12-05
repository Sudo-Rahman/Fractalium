#ifndef MANDELBROT_MAINWINDOW_H
#define MANDELBROT_MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QPoint>
#include <QImage>
#include <QMenuBar>
#include "FractalWidget.hpp"
#include "Fractal.hpp"
#include "Image.hpp"

struct history {
    QImage image;
    std::pair<Fractalium::Double, Fractalium::Double> offset;
    Fractalium::Double step_coord;

};

class MainWindow : public QMainWindow {
Q_OBJECT

    QMenuBar *_menu_bar;

    QImage *_image;
    Fractalium::FractalWidget *_label;

    std::vector<QColor> _color_map;
    int DISPLAY_SIZE = 400;


    std::pair<Fractalium::Double, Fractalium::Double> _offset = {-2.1, -2};

    Fractalium::Double _step_coord;

    Fractalium::Fractal *_fractal;

    Fractalium::Image _diergence_image;

    std::vector<history> _back_history, _front_history;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

private:
    void paintFractal();

    void newSelection(const QPoint &start, const QPoint &end);

    void setupUi();

    void mpiCalculate();

    void back();

    void front();

    void saveImage();

protected:
    bool event(QEvent *event) override;
};

class PaintFractalEvent : public QEvent {
public:
    static const QEvent::Type PaintFractalEventType = static_cast<QEvent::Type>(QEvent::User + 1);

    explicit PaintFractalEvent() : QEvent(PaintFractalEventType)
    {}

    ~PaintFractalEvent() override = default;
};


#endif //MANDELBROT_MAINWINDOW_H
