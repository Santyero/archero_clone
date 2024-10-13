#pragma once
#include "RendererPort.h"
#include "TextureManager.h"
#include <vector>
#include <string>

namespace Game {
    class MainMenu {
    private:
        RendererPort* rendererPort;
        TextureManager* textureManager;
        std::vector<std::string> options;
        int selectedOption;
        SDL_Texture* backgroundTexture;

        void renderTransparentBackground(int x, int y, int width, int height);

    public:
        MainMenu(RendererPort* rendererPort, TextureManager* textureManager);
        void render();
        void handleInput(SDL_Event& event);
        int getSelectedOption() const { return selectedOption; }
		void setSelectedOption(int option) { selectedOption = option; }
    };
}