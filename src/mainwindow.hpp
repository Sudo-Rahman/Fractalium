#ifndef MANDELBROT_MAINWINDOW_H
#define MANDELBROT_MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QMouseEvent>
#include <QProgressBar>
#include <QImage>
#include <QMenuBar>
#include <QStatusBar>
#include <FractalWidget.hpp>
#include <Fractal.hpp>
#include <Image.hpp>
#include <History.hpp>


class MainWindow : public QMainWindow {
Q_OBJECT

    QMenuBar *_menu_bar;

    QStatusBar *_status_bar;

    QImage *_image;

    Fractalium::FractalWidget *_label;

    std::vector<QColor> _color_map;

    uint16_t DISPLAY_SIZE_WIDTH = 400;
    uint16_t DISPLAY_SIZE_HEIGHT = 450;

    std::pair<Fractalium::Double, Fractalium::Double> _offset = {-2.1, -2};

    Fractalium::Double _step_coord;

    Fractalium::Fractal *_fractal;

    Fractalium::Image _divergence_image;

    std::vector<Fractalium::History> _back_history, _front_history;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    static const int TOTAL_COLORS = 500;

private:
    void paintFractal();

    void newSelection(const QPoint &start, const QPoint &end);

    void setupUi();

    void mpiCalculate();

    void back();

    void front();

    void saveImage();

    void updateColor();

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
