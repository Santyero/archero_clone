#pragma once
#include "VisualElement.h"
#include "TextureManager.h"

namespace Game {
    class HUD : public VisualElement {
    private:
        int currentLevel;
        int currentPoints;
        TextureManager* textureManager;
        std::string bannerTextureId;
        Vector pointsPosition;

    public:
        HUD(RendererPort* rendererPort, TextureManager* textureManager);
        void update() override;
        void onCollision(VisualElement* otherElement) override;
        void setLevel(int level);
        void setPoints(int points);
        void renderElement() override;
    };
}