//
// Created by sr-71 on 19/12/2023.
//

#ifndef FRACTALIUM_SNAPSHOTDIALOG_HPP
#define FRACTALIUM_SNAPSHOTDIALOG_HPP

#include <QPushButton>
#include <QDialog>
#include <History.hpp>

class SnapshotDialog : public QDialog {
Q_OBJECT

public:
    explicit SnapshotDialog(Fractalium::SnapshotHistory &snapshotHistory, QWidget *parent);

    enum class Return {
        Import,
        Make,
        None
    };

    [[nodiscard]] const Return &returnType() const
    {
        return _return;
    }

private:
    QPushButton *_import_snapshot, *make_snapshot;
    Return _return{Return::None};

};


#endif //FRACTALIUM_SNAPSHOTDIALOG_HPP
