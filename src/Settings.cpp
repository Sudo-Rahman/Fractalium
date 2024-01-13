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
uint32_t Settings::NODES = 0;

bool Settings::AUTO_SNAPSHOTS = false;
std::string Settings::SAVE_PATH = "./";
const std::string Settings::CRASH_SNAP_PATH = "./snapshot_crash.fractalium";
bool Settings::IS_CRASHED = false;
const std::string Settings::GITHUB_URL = "https://github.com/Sudo-Rahman/Fractalium";
Settings::CalculationType Settings::CALCULATION_TYPE = Settings::CalculationType::COLLUMNS;

void Settings::saveSettings()
{
    QSettings settings;
    settings.setValue("display_size_width", DISPLAY_SIZE_WIDTH);
    settings.setValue("display_size_height", DISPLAY_SIZE_HEIGHT);
    settings.setValue("iterations", ITERATIONS);
    settings.setValue("auto_snapshots", AUTO_SNAPSHOTS);
    settings.setValue("save_path", QString::fromStdString(SAVE_PATH));
    settings.setValue("is_crashed", IS_CRASHED);
    settings.setValue("calculation_type", CALCULATION_TYPE);
}


/**
 * @brief Fonction qui initialise les paramètres de l'application à partir des paramètres sauvegardés dans le fichier de configuration de l'application
 */
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
    CALCULATION_TYPE = static_cast<CalculationType>(settings.value("calculation_type", CALCULATION_TYPE).toInt());

}


/**
 * @brief Fonction qui permet de réinitialiser les paramètres de l'application
 */
void Settings::resetCrash()
{
    IS_CRASHED = false;
    QSettings settings;
    settings.setValue("is_crashed", false);
}

/**
 * @brief Fonction qui permet de définir que l'application a crashé
 */
void Settings::setCrash()
{
    IS_CRASHED = true;
    QSettings settings;
    settings.setValue("is_crashed", true);
}
