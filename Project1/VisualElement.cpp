#include "VisualElement.h"

using namespace Game;

VisualElement::VisualElement(
    RendererPort* adapter, const RenderDataDTO& renderDataDTO
) : rendererPort(adapter) {
    this->positionXInMeters = renderDataDTO.positionXInMeters;
    this->positionYInMeters = renderDataDTO.positionYInMeters;
    this->widthInMeters = renderDataDTO.widthInMeters;
    this->heightInMeters = renderDataDTO.heightInMeters;
    this->hexColor = renderDataDTO.hexColor;
}

void VisualElement::checkCollision(VisualElement* otherElement) {

    if (this->positionXInMeters < otherElement->positionXInMeters + otherElement->widthInMeters &&
        this->positionXInMeters + this->widthInMeters > otherElement->positionXInMeters &&
        this->positionYInMeters < otherElement->positionYInMeters + otherElement->heightInMeters &&
        this->heightInMeters + this->positionYInMeters > otherElement->positionYInMeters) {

        // TODO - FAZER DEPOIS O BLOQUEIO DE COLISAO DO INIMIGO > CASO CONTRARIO O INIMIGO VAI EMPURRAR O PLAYER
        float overlapLeft = (this->positionXInMeters + this->widthInMeters) - otherElement->positionXInMeters;
        float overlapRight = (otherElement->positionXInMeters + otherElement->widthInMeters) - this->positionXInMeters;
        float overlapTop = (this->positionYInMeters + this->heightInMeters) - otherElement->positionYInMeters;
        float overlapBottom = (otherElement->positionYInMeters + otherElement->heightInMeters) - this->positionYInMeters;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        // Descomente o código de resolução de colisão se necessário
        if (minOverlapX < minOverlapY) {
            if (overlapLeft < overlapRight) {
                this->positionXInMeters -= overlapLeft;
            }
            else {
                this->positionXInMeters += overlapRight;
            }
        }
        else {
            if (overlapTop < overlapBottom) {
                this->positionYInMeters -= overlapTop;
            }
            else {
                this->positionYInMeters += overlapBottom;
            }
        }
        
        this->onCollision(otherElement);
    }
}