//
// Created by sr-71 on 18/12/2023.
//

#include "ResizeDialog.hpp"

#include <QLayout>
#include <QLabel>

ResizeDialog::ResizeDialog(const QSize &current, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Resize mainwindow");

    auto layout = new QGridLayout(this);

    layout->addWidget(new QLabel("width"), 0, 0, 1, 1);

    _width = new QSpinBox(this);
    _width->setRange(400, 5120);
    _width->setValue(current.width());
    layout->addWidget(_width, 0, 1, 1, 1);


    layout->addWidget(new QLabel("width"), 1, 0, 1, 1);

    _height = new QSpinBox(this);
    _height->setRange(400, 5120);
    _height->setValue(current.height());
    layout->addWidget(_height, 1, 1, 1, 1);

    _save = new QPushButton("Save");
    layout->addWidget(_save, 2, 0, 1, 2);

    connect(_save, &QPushButton::clicked, this, [this]
    {
        _h = _height->value();
        _w = _width->value();
        _return = Return::OK;
        close();
    });
}
