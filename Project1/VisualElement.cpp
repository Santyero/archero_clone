#include "VisualElement.h"
#include "math-vector.h"

namespace Game
{
    VisualElement::VisualElement(
        RendererPort* adapter, const RenderDataDTO& renderDataDTO
    ) : rendererPort(adapter) {
        this->position = renderDataDTO.position;

        this->size = renderDataDTO.size;
        this->velocity = renderDataDTO.velocity;
        this->hexColor = renderDataDTO.hexColor;
        this->image = renderDataDTO.image;
    }

    void VisualElement::setFrames(const std::vector<SDL_Rect>& newFrames) {
        this->frames = newFrames;
    }

    void VisualElement::updateAnimation() {
        if (frames.empty()) {
            std::cerr << "Error: No frames available for animation." << std::endl;
            return;
        }
        
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastAnimationTime + animationSpeed) {
            lastAnimationTime = currentTime;
            animationFrame++;
            if (animationFrame >= frames.size()) {
                animationFrame = 0;
            }
            srcRect = frames[animationFrame];
        }
    }

}