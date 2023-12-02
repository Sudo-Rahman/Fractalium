//
// Created by sr-71 on 02/12/2023.
//

#include "FractalWidget.hpp"

#include <QMouseEvent>

using namespace Fractalium;

FractalWidget::FractalWidget(QWidget *parent) : QLabel(parent)
{
    _rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
}

void FractalWidget::mousePressEvent(QMouseEvent *event)
{
    _rubber_band_selection = true;
    _start_point = event->pos();
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!_rubber_band_selection)
        return;
    _rubber_band_selection = false;
    _rubber_band->hide();

    if ((event->pos().x() < 0
         || event->pos().y() < 0
         || event->pos().x() > QWidget::width()
         || event->pos().y() > QWidget::height())
         )
        return;

    _release_point = event->pos();

    if (_release_point.x() < _start_point.x())
        std::swap(_release_point, _start_point);


    emit newSelection(_start_point, _release_point);
}

void FractalWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!_rubber_band_selection)
        return;
    _rubber_band->setGeometry(QRect(_start_point, event->pos()).normalized());
    _rubber_band->show();
}

void FractalWidget::back()
{
    if (!_history.empty())
    {
        _history.pop_back();
        setPixmap(QPixmap::fromImage(_history.back()));
    }
}

void FractalWidget::setPixmap(const QPixmap &pixmap)
{
    QLabel::setPixmap(pixmap);
}

void FractalWidget::setFractal(const QImage &image)
{
    _history.push_back(image);
    setPixmap(QPixmap::fromImage(image));
}

