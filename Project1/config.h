#pragma once
#include "math-vector.h"

namespace Game {
    namespace Config {
        static const Vector windowSize{ 800, 600 };
        static const Vector sceneSize{ 800, 600 };
        static const Vector scenePosition{
            (windowSize.x - sceneSize.x) / 2,
            (windowSize.y - sceneSize.y) / 2
        };
    }
}