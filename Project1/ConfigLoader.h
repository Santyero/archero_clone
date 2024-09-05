#pragma once
#include "AnimationConfig.h"

namespace Game
{
    class ConfigLoader {
    public:
        static AnimationConfig loadPlayerAnimationConfig();
    };
}