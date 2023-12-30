//
// Created by sr-71 on 30/12/2023.
//

#ifndef FRACTALIUM_SETTINGSDIALOG_HPP
#define FRACTALIUM_SETTINGSDIALOG_HPP


#include <QDialog>
#include <QVBoxLayout>

class SettingsDialog : public QDialog {
Q_OBJECT

public:

    explicit SettingsDialog(QWidget *parent = nullptr);


private:
    QSize _size;
    int _iterations;
    bool _auto_snapshots;
    QString _save_path;

    QVBoxLayout *_layout;

    void initUi();

    void initResizeUi();

    void initIterationsUi();

    void initAutoSnapshotsUi();

};


#endif //FRACTALIUM_SETTINGSDIALOG_HPP
