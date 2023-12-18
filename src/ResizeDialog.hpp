//
// Created by sr-71 on 18/12/2023.
//

#ifndef FRACTALIUM_RESIZEDIALOG_HPP
#define FRACTALIUM_RESIZEDIALOG_HPP

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>

class ResizeDialog : public QDialog {
Q_OBJECT

public:

    enum class Return {
        OK, NO_OK
    };

    explicit ResizeDialog(uint16_t min, uint16_t max, QWidget *parent = nullptr);

    [[nodiscard]] Return returnType() const
    { return _return; }

    [[nodiscard]] QSize returnSize() const {
        return {_w,_h};
    }

private:
    QPushButton *_save;
    QSpinBox *_width, *_height;
    uint16_t _w,_h;
    Return _return{Return::NO_OK};

};


#endif //FRACTALIUM_RESIZEDIALOG_HPP
