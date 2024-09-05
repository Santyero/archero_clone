#pragma once
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include "math-vector.h"
#include "TextureManager.h"
#include <SDL_image.h>
#include <string>

namespace Game
{
    enum class AnimationState {
        IDLE,
        RUNNING,
        SHOOTING
    };

    using Vector = Mylib::Math::Vector<float, 2>;

    class VisualElement
    {
    protected:
        RendererPort* rendererPort;
        TextureManager* textureManager;
        std::string textureId;
        std::string hexColor;
        bool deleted = false;
        Vector size;
        Vector position;
        Vector velocity;

        AnimationState currentState = AnimationState::IDLE;
        size_t currentFrame = 0;
        Uint32 lastAnimationUpdate = 0;
        Uint32 animationSpeed = 100;

    public:
        VisualElement(RendererPort* adapter, TextureManager* textureManager, const std::string& textureId, const RenderDataDTO& renderDataDTOParam);
        virtual ~VisualElement() = default;

        void setAnimationState(AnimationState state);

        virtual void renderElement();
        virtual void update();
        virtual void onCollision(VisualElement* otherElement) = 0;

        bool checkCollision(VisualElement* otherElement);
        void preventTranposition(VisualElement* otherElement);

        Vector getPosition() const { return this->position; }
        Vector getSize() const { return this->size; }
        Vector getVelocity() const { return this->velocity; }

        void setVelocity(const Vector& velocity) { this->velocity = velocity; }
        void setPosition(const Vector& position) { this->position = position; }
        void setSize(const Vector& size) { this->size = size; }
        void setHexColor(const std::string& hexColor) { this->hexColor = hexColor; }

        void physicsUpdate(float deltaTime);

        bool isDeleted() const { return this->deleted; }
        void destroy() { this->deleted = true; }

    protected:
        void updateAnimation();
        std::string getCurrentAnimationState() const;
    };
}