#include "HUD.h"

namespace Game {
    HUD::HUD(RendererPort* rendererPort, TextureManager* textureManager)
        : VisualElement(rendererPort, textureManager, "hud_banner",
            RenderDataDTO{ {10, 10}, {200, 50}, {0, 0}, "#FFFFFF" }),
        currentLevel(1),
        textureManager(textureManager),
        bannerTextureId("hud_banner") {
    }

    void HUD::update() {}
    void HUD::onCollision(VisualElement* otherElement) {}

    void HUD::setLevel(int level) {
        currentLevel = level;
    }

    void HUD::renderElement() {
        // Renderizar o banner

        SDL_Texture* bannerTexture = textureManager->getTexture(bannerTextureId, "idle", 0);
        if (!bannerTexture) {
            std::cerr << "Textura do banner não encontrada!" << std::endl;
            return;
        }

        SDL_Rect destRect = {
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)
        };

        rendererPort->renderTexture(bannerTexture, NULL, &destRect);

        std::string levelText = "Level: " + std::to_string(currentLevel);
        SDL_Color textColor = { 255, 0, 0, 255 };
        rendererPort->renderSimpleText(levelText,
            static_cast<int>(position.x + 20),
            static_cast<int>(position.y + 50),
            textColor);
    }

}