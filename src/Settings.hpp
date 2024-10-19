//
// Created by sr-71 on 30/12/2023.
//

#ifndef FRACTALIUM_SETTINGS_HPP
#define FRACTALIUM_SETTINGS_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace Fractalium {
    struct Settings {

        static uint16_t DISPLAY_SIZE_WIDTH;
        static uint16_t DISPLAY_SIZE_HEIGHT;

        static uint32_t ITERATIONS;

        static uint32_t NODES;

        static bool AUTO_SNAPSHOTS;
        static std::string SAVE_PATH;

        static bool IS_CRASHED;
        const static std::string CRASH_SNAP_PATH;

        enum AreaAlgorithmType : uint8_t {
            COLLUMNS,
            SQUARES,
        };

        static AreaAlgorithmType AREA_ALGORITHM_TYPE;

        static void saveSettings();

        static void init();

        static void resetCrash();

        static void setCrash();

        static const std::string GITHUB_URL;
    };
}

#endif //FRACTALIUM_SETTINGS_HPP
