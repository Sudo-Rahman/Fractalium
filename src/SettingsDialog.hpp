//
// Created by sr-71 on 30/12/2023.
//

#ifndef FRACTALIUM_SETTINGSDIALOG_HPP
#define FRACTALIUM_SETTINGSDIALOG_HPP


#include <QDialog>
#include <QVBoxLayout>
#include <Settings.hpp>


class SettingsDialog : public QDialog {
Q_OBJECT

public:

    explicit SettingsDialog(QWidget *parent = nullptr);


private:
    QSize _size;
    int _iterations;
    bool _auto_snapshots;
    QString _save_path;
    Fractalium::Settings::AreaAlgorithmType area_algorithm_type;


    QVBoxLayout *_layout;

    void initUi();

    void initResizeUi();

    void initIterationsUi();

    void initAutoSnapshotsUi();

    void initAreaAlgorithmTypeUi();

};


#endif //FRACTALIUM_SETTINGSDIALOG_HPP
