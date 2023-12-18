//
// Created by sr-71 on 18/12/2023.
//

#ifndef FRACTALIUM_ITERATIONDIALOG_HPP
#define FRACTALIUM_ITERATIONDIALOG_HPP

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>

class IterationDialog : public QDialog {
Q_OBJECT

public:

    explicit IterationDialog(uint64_t val, QWidget *parent);

    enum class Return {
        OK, NO_OK
    };

    [[nodiscard]] const uint64_t &iteration() const
    {
        return _it;
    }

    [[nodiscard]] const Return &returnType() const
    {
        return _return;
    }

private:

    QSlider *_iteration;
    QSpinBox *_iteration_spin;
    uint64_t _it;
    QPushButton *_save;
    Return _return{Return::NO_OK};

};


#endif //FRACTALIUM_ITERATIONDIALOG_HPP
