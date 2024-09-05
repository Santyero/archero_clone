#include "ConfigLoader.h"
#include <iostream>

namespace Game {

    AnimationConfig ConfigLoader::loadPlayerAnimationConfig() {
        AnimationConfig config;

        // Inicializar quadros de animação
        config.idleFrames = {
            {0, 0, 64, 64},
            {64, 0, 64, 64}
        };

        config.runningFrames = {
            {0, 64, 64, 64},
            {64, 64, 64, 64},
            {128, 64, 64, 64},
            {192, 64, 64, 64},
            {256, 64, 64, 64},
            {320, 64, 64, 64},
            {384, 64, 64, 64},
            {448, 64, 64, 64}
        };

        config.shootingFrames = {
            {0, 128, 64, 64},
            {64, 128, 64, 64}
        };

        return config;
    }
}