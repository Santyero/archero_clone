#pragma once
#include "RendererPort.h"
#include "TextureManager.h"
#include "MixerManager.h"
#include <memory>
#include <SDL.h>

namespace Game {
    class OptionsMenu {
    private:
        RendererPort* rendererPort;
        std::unique_ptr<TextureManager> textureManager;
        MixerManager* mixerManager;
        int selectedOption;
        int volumeLevel;

        void renderOption(const std::string& text, int yPos, bool isSelected);
        void renderVolumeBar();
		bool isFullscreen;

    public:
        OptionsMenu(RendererPort* rendererPort, TextureManager* textureManager, MixerManager* mixerManager, bool isFullscreen);
        void handleInput(SDL_Event& event);
        void render();
        int getSelectedOption() const { return selectedOption; }
        void adjustVolume(int change);
        void toggleFullscreen();
        bool getIsFullscreen() const { return isFullscreen; }
    };
}