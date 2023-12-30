//
// Created by sr-71 on 30/12/2023.
//
#include <Settings.hpp>
#include <QCoreApplication>
#include <QSettings>


using namespace Fractalium;

uint16_t Settings::DISPLAY_SIZE_WIDTH = 500;
uint16_t Settings::DISPLAY_SIZE_HEIGHT = 500;

uint32_t Settings::ITERATIONS = 100;

bool Settings::AUTO_SNAPSHOTS = false;
std::string Settings::SAVE_PATH = "./";
const std::string Settings::CRASH_SNAP_PATH = "./snapshot_crash.fractalium";
bool Settings::IS_CRASHED = false;

void Settings::saveSettings()
{
    QSettings settings;
    settings.setValue("display_size_width", DISPLAY_SIZE_WIDTH);
    settings.setValue("display_size_height", DISPLAY_SIZE_HEIGHT);
    settings.setValue("iterations", ITERATIONS);
    settings.setValue("auto_snapshots", AUTO_SNAPSHOTS);
    settings.setValue("save_path", QString::fromStdString(SAVE_PATH));
    settings.setValue("is_crashed", IS_CRASHED);
}

void Settings::init()
{
    QCoreApplication::setOrganizationName("Fractalium Inc.");
    QCoreApplication::setApplicationName("Fractalium");

    QSettings settings;
    DISPLAY_SIZE_WIDTH = settings.value("display_size_width", DISPLAY_SIZE_WIDTH).toUInt();
    DISPLAY_SIZE_HEIGHT = settings.value("display_size_height", DISPLAY_SIZE_HEIGHT).toUInt();
    ITERATIONS = settings.value("iterations", ITERATIONS).toUInt();
    AUTO_SNAPSHOTS = settings.value("auto_snapshots", AUTO_SNAPSHOTS).toBool();
    SAVE_PATH = settings.value("save_path", QString::fromStdString(SAVE_PATH)).toString().toStdString();
    IS_CRASHED = settings.value("is_crashed", IS_CRASHED).toBool();
}

void Settings::resetCrash()
{
    IS_CRASHED = false;
    QSettings settings;
    settings.setValue("is_crashed", false);
}

void Settings::setCrash()
{
    IS_CRASHED = true;
    QSettings settings;
    settings.setValue("is_crashed", true);
}
