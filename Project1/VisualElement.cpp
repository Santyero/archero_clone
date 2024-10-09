#include "VisualElement.h"
#include "math-vector.h"
#include <algorithm>
#include "GameStateManager.h"

namespace Game
{
    VisualElement::VisualElement(RendererPort* adapter, TextureManager* textureManager, const std::string& textureId, const RenderDataDTO& renderDataDTO)
        : rendererPort(adapter),
        textureManager(textureManager),
        textureId(textureId),
        position(renderDataDTO.position),
        size(renderDataDTO.size),
        velocity(renderDataDTO.velocity),
        hexColor(renderDataDTO.hexColor),
        currentState(AnimationState::IDLE)
    {
    }

    void VisualElement::setAnimationState(AnimationState state) {
        if (this->currentState != state) {
            this->currentState = state;
            this->currentFrame = 0;
            this->lastAnimationUpdate = SDL_GetTicks();
        }
    }

    void VisualElement::renderElement() {
        SDL_Rect destRect = {
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)
        };
        bool flipHorizontally = velocity.x < 0;
        SDL_Texture* texture = textureManager->getTexture(textureId, getCurrentAnimationState(), currentFrame);

        this->rendererPort->renderElement(RenderDataDTO{
            this->position,
            this->size,
            this->velocity,
            this->hexColor,
            texture,
            flipHorizontally,
        });
    }

    void VisualElement::update() {
        updateAnimation();
    }

    void VisualElement::updateAnimation() {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastAnimationUpdate > frameTime) {
            size_t frameCount = textureManager->getFrameCount(textureId, getCurrentAnimationState());
            if (frameCount > 0) {
                currentFrame = (currentFrame + 1) % frameCount;
            }
            lastAnimationUpdate = currentTime;
        }
    }

    std::string VisualElement::getCurrentAnimationState() const {
        switch (currentState) {
        case AnimationState::IDLE: return "idle";
		case AnimationState::WALK: return "walk";
		case AnimationState::SHOOT: return "shoot";
		case AnimationState::SHOOT_TOP: return "shootTop";
		case AnimationState::SHOOT_ANGULAR_TOP: return "shootAngularTop";
		case AnimationState::SHOOT_ANGULAR_BOTTOM: return "shootAngularBottom";
		case AnimationState::SHOOT_BOTTOM: return "shootBottom";
        default: return "idle";
        }
    }

    bool VisualElement::checkCollision(VisualElement* otherElement) {
        Vector otherElementPosition = otherElement->getPosition();
        Vector otherElementSize = otherElement->getSize();

        return (this->position.x < otherElementPosition.x + otherElementSize.x &&
            this->position.x + this->size.x > otherElementPosition.x &&
            this->position.y < otherElementPosition.y + otherElementSize.y &&
            this->size.y + this->position.y > otherElementPosition.y);
    }

    void VisualElement::preventTranposition(VisualElement* otherElement) {
        Vector otherElementPosition = otherElement->getPosition();
        Vector otherElementSize = otherElement->getSize();

        float overlapLeft = (this->position.x + this->size.x) - otherElementPosition.x;
        float overlapRight = (otherElementPosition.x + otherElementSize.x) - this->position.x;
        float overlapTop = (this->position.y + this->size.y) - otherElementPosition.y;
        float overlapBottom = (otherElementPosition.y + otherElementSize.y) - this->position.y;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY) {
            this->position.x += (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
        }
        else {
            this->position.y += (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
        }

        this->onCollision(otherElement);
    }

    void VisualElement::physicsUpdate(float deltaTime) {
        this->position += this->velocity * deltaTime;
    }
}