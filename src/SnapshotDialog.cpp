//
// Created by sr-71 on 19/12/2023.
//

#include "SnapshotDialog.hpp"
#include <QFileDialog>
#include <qgridlayout.h>

SnapshotDialog::SnapshotDialog(Fractalium::SnapshotHistory &snapshotHistory, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Instatané");

    setFixedHeight(100);

    auto layout = new QGridLayout(this);

    _import_snapshot = new QPushButton("Importer un instatané", this);
    _import_snapshot->setToolTip("Importer un instatané");
    layout->addWidget(_import_snapshot, 0, 0, 1, 1);

    make_snapshot = new QPushButton("Faire un instatané", this);
    make_snapshot->setToolTip("Faire un instatané");
    layout->addWidget(make_snapshot, 0, 1, 1, 1);

    connect(_import_snapshot, &QPushButton::clicked, this, [this, &snapshotHistory]()
    {
        QString filename = QFileDialog::getOpenFileName(this, tr("Importer un instatané"),
                                                        QDir::homePath(),
                                                        tr("(*.fractalium)"));
        if (filename.isEmpty())
            return;
        Fractalium::importSnapshot(filename.toStdString(), snapshotHistory);
        _return = Return::Import;
        close();
    });

    connect(make_snapshot, &QPushButton::clicked, this, [this, &snapshotHistory]()
    {
        auto dateTime = QDateTime::currentDateTime();
        QString filename = QFileDialog::getSaveFileName(this, tr("Sauvegarder un instatané"),
                                                        QDir::homePath() + "/fractalium_" +
                                                        dateTime.toString("dd_MM_yyyy_hh_mm_ss") + ".fractalium",
                                                        tr("(*.fractalium)"));
        if (filename.isEmpty())
            return;
        Fractalium::makeSnapshot(filename.toStdString(), snapshotHistory);
        _return = Return::Make;
        close();
    });
}