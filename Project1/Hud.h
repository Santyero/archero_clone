#pragma once
#include "VisualElement.h"
#include "TextureManager.h"
#include <string>

namespace Game {
    class HUD : public VisualElement {
    private:
        int currentLevel;
        TextureManager* textureManager;
        std::string bannerTextureId;

    public:
        HUD(RendererPort* rendererPort, TextureManager* textureManager);

        void update() override;
        void onCollision(VisualElement* otherElement) override;
        void renderElement() override;

        void setLevel(int level);
    };
}
