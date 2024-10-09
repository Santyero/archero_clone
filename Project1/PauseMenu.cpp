#include "PauseMenu.h"
#include "config.h"

namespace Game {
    PauseMenu::PauseMenu(RendererPort* rendererPort, std::unique_ptr<TextureManager, std::default_delete<TextureManager>> textureManager)
        : VisualElement(rendererPort, textureManager.get(), "pause_menu",
            RenderDataDTO{ {Config::windowSize.x / 2 - 150, Config::windowSize.y / 2 - 200}, {300, 400}, {0, 0}, "#000000" }),
        textureManager(std::move(textureManager)), selectedOption(0) {
        options = {
            {"Resume", {static_cast<int>(position.x) + 20, static_cast<int>(position.y) + 20, 260, 40}},
            {"Options", {static_cast<int>(position.x) + 20, static_cast<int>(position.y) + 70, 260, 40}},
            {"Quit", {static_cast<int>(position.x) + 20, static_cast<int>(position.y) + 120, 260, 40}}
        };
    }

    void PauseMenu::update() {
        // Nada a atualizar no menu de pausa
    }

    void PauseMenu::renderElement() {
        // Renderizar fundo semi-transparente
        SDL_SetRenderDrawBlendMode(rendererPort->getRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rendererPort->getRenderer(), 0, 0, 0, 128);
        SDL_Rect fullscreen = { 0, 0, Config::windowSize.x, Config::windowSize.y };
        SDL_RenderFillRect(rendererPort->getRenderer(), &fullscreen);

        // Renderizar opções do menu
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Color selectedColor = { 255, 255, 0, 255 };
        for (int i = 0; i < options.size(); ++i) {
            SDL_Color color = (i == selectedOption) ? selectedColor : textColor;
            rendererPort->renderSimpleText(options[i].text, options[i].rect.x, options[i].rect.y, color);
        }
    }

    void PauseMenu::handleInput(SDL_Event& event) {
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

    int PauseMenu::handleMouseClick(int x, int y) {
        for (int i = 0; i < options.size(); ++i) {
            if (x >= options[i].rect.x && x <= options[i].rect.x + options[i].rect.w &&
                y >= options[i].rect.y && y <= options[i].rect.y + options[i].rect.h) {
                return i;
            }
        }
        return -1;
    }
}