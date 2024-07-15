#include "VisualElement.h"

namespace Game
{
    VisualElement::VisualElement(
        RendererPort* adapter, const RenderDataDTO& renderDataDTO
    ) : rendererPort(adapter) {
        this->position = renderDataDTO.position;

        this->size = renderDataDTO.size;
        this->hexColor = renderDataDTO.hexColor;
    }

}