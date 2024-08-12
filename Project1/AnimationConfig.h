#pragma once
#include <vector>
#include <SDL.h>

struct AnimationConfig {
    std::vector<SDL_Rect> idleFrames;
    std::vector<SDL_Rect> runningFrames;
    std::vector<SDL_Rect> shootingFrames;
};