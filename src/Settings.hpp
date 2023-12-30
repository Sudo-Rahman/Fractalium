//
// Created by sr-71 on 30/12/2023.
//

#ifndef FRACTALIUM_SETTINGS_HPP
#define FRACTALIUM_SETTINGS_HPP

#include <cstdint>
#include <string>

namespace Fractalium
{
    struct Settings {

        static uint16_t DISPLAY_SIZE_WIDTH;
        static uint16_t DISPLAY_SIZE_HEIGHT;

        static uint32_t ITERATIONS;

        static bool AUTO_SNAPSHOTS;
        static std::string SAVE_PATH;

        static bool IS_CRASHED;
        const static std::string CRASH_SNAP_PATH ;

        static void saveSettings();

        static void init();

        static void resetCrash();

        static void setCrash();
    };
}

#endif //FRACTALIUM_SETTINGS_HPP
