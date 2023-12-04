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

class MainWindow : public QMainWindow {
    Q_OBJECT

    QMenuBar *_menu_bar;

    QImage *image;
    Fractalium::FractalWidget *label;

    std::vector<QColor> _color_map;
    int DISPLAY_SIZE = 400;


    Fractalium::Double _step_coord;

    Fractalium::Fractal *fractal;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    void paintFractal();

    void newSelection(const QPoint &start, const QPoint &end);

    void setupUi();

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
