#include "MainMenu.h"
#include "config.h"

namespace Game {
    MainMenu::MainMenu(RendererPort* rendererPort, TextureManager* textureManager)
        : rendererPort(rendererPort), textureManager(textureManager), selectedOption(0) {
        options = { "New Game", "Options", "High Scores", "Credits",  "Exit"};
        backgroundTexture = textureManager->getTexture("main_menu_background", "idle", 0);
    }

    void MainMenu::renderTransparentBackground(int x, int y, int width, int height) {
        SDL_Renderer* renderer = rendererPort->getRenderer();
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // RGBA: Black with 50% opacity
        SDL_Rect rect = { x, y, width, height };
        SDL_RenderFillRect(renderer, &rect);
    }

    void MainMenu::render() {
        // Render background
        if (backgroundTexture) {
            SDL_Rect destRect = { 0, 0, Config::windowSize.x, Config::windowSize.y };
            rendererPort->renderTexture(backgroundTexture, nullptr, &destRect);
        }
        else {
            // Fallback to black background if texture is not available
            SDL_SetRenderDrawColor(rendererPort->getRenderer(), 0, 0, 0, 255);
            SDL_RenderClear(rendererPort->getRenderer());
        }

        // Calculate menu dimensions
        int menuWidth = 250;
        int menuHeight = options.size() * 50 + 20; // 50 pixels per option + 20 pixels padding
        int startX = Config::windowSize.x / 2 - menuWidth / 2;
        int startY = Config::windowSize.y / 2 - menuHeight / 2;

        // Render transparent background for menu options
        renderTransparentBackground(startX, startY, menuWidth, menuHeight);

        // Render menu options
        for (size_t i = 0; i < options.size(); ++i) {
            SDL_Color color = (i == selectedOption) ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
            rendererPort->renderSimpleText(options[i], startX + 20, startY + 20 + i * 50, color);
        }
    }

    void MainMenu::handleInput(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = (selectedOption - 1 + options.size()) % options.size();
                break;
            case SDLK_DOWN:
                selectedOption = (selectedOption + 1) % options.size();
                break;
            }
        }
    }
}