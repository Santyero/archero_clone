#pragma once
#include "VisualElement.h"
#include "TextureManager.h"
#include <string>

namespace Game {
    class HUD : public VisualElement {
    public:
        HUD(RendererPort* rendererPort, TextureManager* textureManager);
        void setLevel(int level);
        void renderElement() override;
        void update() override;
        void onCollision(VisualElement* otherElement) override;

    private:
        int currentLevel;
        TextureManager* textureManager;
        std::string bannerTextureId;
    };
}
