#include "HUD.h"
#include "ScoreManager.h"
#include <iostream>
#include "config.h"

namespace Game {
    HUD::HUD(RendererPort* rendererPort, TextureManager* textureManager)
        : VisualElement(rendererPort, textureManager, "hud_banner",
            RenderDataDTO{ {10, 10}, {250, 100}, {0, 0}, "#FFFFFF" }),
        currentLevel(1),
        currentPoints(0),
        textureManager(textureManager),
        bannerTextureId("hud_banner") {
        pointsPosition = { 10, Config::windowSize.y - 50 };
    }

    void HUD::update() {
        currentPoints = ScoreManager::getInstance()->getPoints();
    }

    void HUD::onCollision(VisualElement* otherElement) {}

    void HUD::setLevel(int level) {
        currentLevel = level;
    }

    void HUD::setPoints(int points) {
        currentPoints = points;
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
            static_cast<int>(size.x + 20),
            static_cast<int>(size.y - 10)
        };
        rendererPort->renderTexture(bannerTexture, NULL, &destRect);

        // Renderizar o nível
        std::string levelText = "Level: " + std::to_string(currentLevel);
        SDL_Color textColor = { 255, 255, 255, 255 };
        rendererPort->renderSimpleText(levelText,
            static_cast<int>(position.x) + 20,
            static_cast<int>(position.y) + 15,
            textColor);

		SDL_Texture* pointstexture = textureManager->getTexture("hud_points", "idle", 0);
		if (!pointstexture) {
			std::cerr << "Textura do banner não encontrada!" << std::endl;
			return;
		}

        SDL_Rect huddestRect = {
            static_cast<int>(pointsPosition.x),
            static_cast<int>(pointsPosition.y - 15),
            static_cast<int>(size.x + 200),
            static_cast<int>(size.y + 100)
        };
        rendererPort->renderTexture(pointstexture, NULL, &huddestRect);

        // Renderizar a pontuação
        std::string pointsText = "Points: " + std::to_string(currentPoints);
        rendererPort->renderSimpleText(pointsText,
            static_cast<int>(pointsPosition.x + 5),
            static_cast<int>(pointsPosition.y + 5),
            textColor);
    }
}