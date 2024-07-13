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

    void VisualElement::checkCollision(VisualElement* otherElement) {

        if (
            (this->position.x < otherElement->position.x + otherElement->size.y)
            && ( this->position.x + this->size.y > otherElement->position.x )
            && ( this->position.y < otherElement->position.y + otherElement->size.x)
            && ( this->size.x + this->position.y > otherElement->position.y )
         ) {

            // TODO - FAZER DEPOIS O BLOQUEIO DE COLISAO DO INIMIGO > CASO CONTRARIO O INIMIGO VAI EMPURRAR O PLAYER
            float overlapLeft = (this->position.x + this->size.y) - otherElement->position.x;
            float overlapRight = (otherElement->position.x + otherElement->size.y) - this->position.x;
            float overlapTop = (this->position.y + this->size.x) - otherElement->position.y;
            float overlapBottom = (otherElement->position.y + otherElement->size.x) - this->position.y;

            float minOverlapX = std::min(overlapLeft, overlapRight);
            float minOverlapY = std::min(overlapTop, overlapBottom);

            // Descomente o código de resolução de colisão se necessário
            if (minOverlapX < minOverlapY) {
                if (overlapLeft < overlapRight) {
                    this->position.x -= overlapLeft;
                }
                else {
                    this->position.x += overlapRight;
                }
            }
            else {
                if (overlapTop < overlapBottom) {
                    this->position.y -= overlapTop;
                }
                else {
                    this->position.y += overlapBottom;
                }
            }

            this->onCollision(otherElement);
        }
    }
}