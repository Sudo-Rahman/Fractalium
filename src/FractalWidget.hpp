//
// Created by sr-71 on 02/12/2023.
//

#ifndef FRACTALIUM_FRACTALWIDGET_HPP
#define FRACTALIUM_FRACTALWIDGET_HPP


#include <QLabel>
#include <QRubberBand>

namespace Fractalium
{

    class FractalWidget : public QLabel {
    Q_OBJECT

        bool _rubber_band_selection = false;

        QRubberBand *_rubber_band;

        QPoint _start_point, _release_point;

        void setPixmap(const QPixmap &pixmap);

    public:
        explicit FractalWidget(QWidget *parent = nullptr);

        ~FractalWidget() override
        = default;

        void setFractal(const QImage &image);

    signals:

        void newSelection(const QPoint &, const QPoint &);

    protected:
        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

    };
}

#endif //FRACTALIUM_FRACTALWIDGET_HPP
