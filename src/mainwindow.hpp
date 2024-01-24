#ifndef MANDELBROT_MAINWINDOW_H
#define MANDELBROT_MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QTimer>
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

    std::pair<Fractalium::Double, Fractalium::Double> _offset = {-2.1, -2};

    Fractalium::Double _step_coord;

    Fractalium::Fractal *_fractal;

    Fractalium::Image _divergence_image;

    std::vector<Fractalium::History> _back_history, _front_history;

    QTimer *_timer;
    uint64_t _counter = 0;
    QLabel *_status_label;

    QString _lauched_time;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    static const int TOTAL_COLORS = 500;

    static MainWindow *instance;

private:
    void paintFractal(bool save);

    void newSelection(const QPoint &start, const QPoint &end);

    void setupUi();

    void mpiCalculate();

    void back();

    void front();

    void saveImage();

    void updateColor();

    void updateFractalColor();

    void loadSnapshot(const Fractalium::SnapshotHistory &snapshot);

    void lauchAfterCrash();

    static void signalSnapshot(int signum, const std::vector<Fractalium::History> &backHistory,
                               const std::vector<Fractalium::History> &frontHistory,const Fractalium::Fractal &fractal);

    static void handleSignal();

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
