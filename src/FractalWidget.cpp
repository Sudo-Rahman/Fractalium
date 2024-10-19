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
FractalWidget::FractalWidget(QWidget *parent) : QLabel(parent) {
    _rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
    _selection_enabled = false;
}

/**
 * @brief Evènement de pression de la souris, permet de définir la zone de sélection de la fractale à afficher
 * @param event
 */
void FractalWidget::mousePressEvent(QMouseEvent *event) {
    if (!_selection_enabled)
        return;
    _rubber_band_selection = true;
    _start_point = event->pos();
}

/**
 * @brief Permet de définir la zone de sélection pour zoomer dans la fractale
 * @param event L'évènement de relâchement de la souris
 */
void FractalWidget::mouseReleaseEvent(QMouseEvent *event) {
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

//    si le point de relâchement est plus petit que le point de départ, on inverse les deux points
    if (_release_point.x() < _start_point.x())
        std::swap(_release_point, _start_point);


    emit newSelection(_start_point, _release_point);
}

/**
 * @brief Dessine le rectangle de sélection sur la fractale
 * @param event L'évènement de déplacement de la souris
 */
void FractalWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!_rubber_band_selection)
        return;
    _rubber_band->setGeometry(QRect(_start_point, event->pos()).normalized());
    _rubber_band->show();
}

/**
 * @brief Permet de définir l'image de la fractale à afficher
 * @param pixmap L'image de la fractale à afficher
 */
void FractalWidget::setPixmap(const QPixmap &pixmap) {
    QLabel::setPixmap(pixmap);
}

/**
 * @brief Permet de définir l'image de la fractale à afficher
 * @param image L'image de la fractale à afficher
 */
void FractalWidget::setFractal(const QImage &image) {
    setPixmap(QPixmap::fromImage(image));
}

