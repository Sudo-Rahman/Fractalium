//
// Created by sr-71 on 02/12/2023.
//

#include "FractalWidget.hpp"

#include <QMouseEvent>

using namespace Fractalium;

/**
 * @brief Constructeur de la classe FractalWidget
 * @param parent Le widget parent
 */
FractalWidget::FractalWidget(QWidget *parent) : QLabel(parent)
{
    _rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
}

/**
 * @brief Destructeur de la classe FractalWidget
 * @param event
 */
void FractalWidget::mousePressEvent(QMouseEvent *event)
{
    _rubber_band_selection = true;
    _start_point = event->pos();
}

/**
 * @brief Permet de définir la zone de sélection de la fractale à afficher
 * @param event L'évènement de relâchement de la souris
 */
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

/**
 * @brief Permet de définir la zone de sélection de la fractale à afficher
 * @param event L'évènement de déplacement de la souris
 */
void FractalWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!_rubber_band_selection)
        return;
    _rubber_band->setGeometry(QRect(_start_point, event->pos()).normalized());
    _rubber_band->show();
}

/**
 * @brief Permet de définir la zone de sélection de la fractale à afficher
 * @param event L'évènement de déplacement de la souris
 */
void FractalWidget::setPixmap(const QPixmap &pixmap)
{
    QLabel::setPixmap(pixmap);
}

/**
 * @brief Permet de définir la zone de sélection de la fractale à afficher
 * @param image L'image de la fractale à afficher
 */
void FractalWidget::setFractal(const QImage &image)
{
    setPixmap(QPixmap::fromImage(image));
}

