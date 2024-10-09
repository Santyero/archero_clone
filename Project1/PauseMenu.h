#pragma once
#include "VisualElement.h"
#include "TextureManager.h"
#include <memory>
#include <vector>

namespace Game {
    struct MenuOption {
        std::string text;
        SDL_Rect rect;
    };

    class PauseMenu : public VisualElement {
    private:
        std::unique_ptr<TextureManager, std::default_delete<TextureManager>> textureManager;
        std::vector<MenuOption> options;
        int selectedOption;

    public:
        PauseMenu(RendererPort* rendererPort, std::unique_ptr<TextureManager, std::default_delete<TextureManager>> textureManager);
        void update() override;
        void onCollision(VisualElement* otherElement) override {}
        void renderElement() override;
        void handleInput(SDL_Event& event);
        int getSelectedOption() const { return selectedOption; }
        int handleMouseClick(int x, int y);
    };
}