#include "OptionsMenu.h"
#include "config.h"
#include "SDLRendererAdapter.h"

namespace Game {
    OptionsMenu::OptionsMenu(RendererPort* rendererPort, TextureManager* textureManager, MixerManager* mixerManager, bool isFullscreen)
        : rendererPort(rendererPort), textureManager(std::make_unique<TextureManager>(*textureManager)),
        mixerManager(mixerManager), selectedOption(0), volumeLevel(100), isFullscreen(isFullscreen) {}

    void OptionsMenu::handleInput(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = (selectedOption - 1 + 3) % 3;
                break;
            case SDLK_DOWN:
                selectedOption = (selectedOption + 1) % 3;
                break;
            case SDLK_LEFT:
                if (selectedOption == 0) adjustVolume(-5);
                break;
            case SDLK_RIGHT:
                if (selectedOption == 0) adjustVolume(5);
                break;
            case SDLK_RETURN:
                if (selectedOption == 1) toggleFullscreen();
                break;
            }
        }
    }

    void OptionsMenu::render() {
        SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());

        renderOption("Volume: " + std::to_string(volumeLevel) + "%", Config::windowSize.y / 2 - 75, selectedOption == 0);
        renderOption("Fullscreen: " + std::string(isFullscreen ? "On" : "Off"), Config::windowSize.y / 2, selectedOption == 1);
        renderOption("Back", Config::windowSize.y / 2 + 75, selectedOption == 2);

        renderVolumeBar();

        rendererPort->renderPresent();
    }

    void OptionsMenu::renderOption(const std::string& text, int yPos, bool isSelected) {
        SDL_Color color = isSelected ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
        rendererPort->renderSimpleText(text, Config::windowSize.x / 2 - 100, yPos, color);
    }

    void OptionsMenu::renderVolumeBar() {
        int barWidth = 200;
        int barHeight = 20;
        int xPos = (Config::windowSize.x - barWidth) / 2;
        int yPos = Config::windowSize.y / 2 - 30;

        SDL_Rect emptyBar = { xPos, yPos, barWidth, barHeight };
        SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 100, 100, 100, 255);
        SDL_RenderFillRect(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), &emptyBar);

        SDL_Rect filledBar = { xPos, yPos, barWidth * volumeLevel / 100, barHeight };
        SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 255, 0, 255);
        SDL_RenderFillRect(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), &filledBar);
    }

    void OptionsMenu::adjustVolume(int change) {
        volumeLevel = std::max(0, std::min(100, volumeLevel + change));
        mixerManager->setVolume(volumeLevel);
    }

    void OptionsMenu::toggleFullscreen() {
        isFullscreen = !isFullscreen;
    }

}