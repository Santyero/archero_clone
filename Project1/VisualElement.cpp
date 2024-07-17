#include "VisualElement.h"

namespace Game
{
    VisualElement::VisualElement(
        RendererPort* adapter, const RenderDataDTO& renderDataDTO
    ) : rendererPort(adapter) {
        this->position = renderDataDTO.position;

        this->size = renderDataDTO.size;
        this->velocity = renderDataDTO.velocity;
        this->hexColor = renderDataDTO.hexColor;
    }

}