//
// Created by sr-71 on 18/12/2023.
//

#include "IterationDialog.hpp"

#include <QLayout>
#include <QLabel>

IterationDialog::IterationDialog(uint64_t def,QWidget *parent) : QDialog(parent)
{
    auto layout = new QGridLayout(this);

    auto label_min = new QLabel("min : 10 ", this);
    int max = std::numeric_limits<int>::max();
    auto label_max = new QLabel("max : "+QString::number(max)+ " ",this);
    auto label_current = new QLabel(this);
    label_current->setAlignment(Qt::AlignCenter);

    _iteration = new QSlider(Qt::Orientation::Horizontal,this);
    _iteration->setRange(10, max);
    _iteration_spin = new QSpinBox(this);
    _iteration_spin->setRange(10, max);
    _save = new QPushButton("Save", this);



    connect(_iteration, &QSlider::valueChanged, this,[=, this](int val){
        label_current->setText("value : "+ QString::number(val));
        _iteration_spin->setValue(val);
    });

    connect(_iteration_spin, &QSpinBox::valueChanged, this,[=, this](int val){
        label_current->setText("value : "+ QString::number(val));
        _iteration->setValue(val);
    });

    _iteration->setValue((int)def);

    connect(_save, &QPushButton::clicked, this, [this]
    {
        _it = _iteration->value();
        _return = Return::OK;
        close();
    });


    layout->addWidget(label_min, 0,0,1,1);
    layout->addWidget(_iteration, 0,1,1,3);
    layout->addWidget(label_max, 0,4,1,1);
    layout->addWidget(label_current, 1,0,1,5);
    layout->addWidget(_iteration_spin, 1,0,1,5);
    layout->addWidget(_save, 3,0,1,5);
}
