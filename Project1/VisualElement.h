#pragma once
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include "math-vector.h"
#include <vector>
#include <SDL_image.h>

namespace Game
{
	using Vector = Mylib::Math::Vector<float, 2>;
	class VisualElement
	{
	protected:
		RendererPort *rendererPort;
		std::string hexColor;
		bool deleted = false;
		Vector size;
		Vector position;
		Vector velocity;
		SDL_Surface* image = nullptr;
		SDL_Rect srcRect;
		int animationFrame = 0;
		int animationSpeed = 100; // Velocidade da animação em milissegundos por frame
		Uint32 lastAnimationTime = 0;
		std::vector<SDL_Rect> frames; // Quadros de animação
	public:
		VisualElement(RendererPort *adapter, const RenderDataDTO &renderDataDTOParam);

		void renderElement()
		{
			updateAnimation();
			//SDL_Rect srcRect = { 0, 0, 64, 64 };
			this->rendererPort->renderElement(RenderDataDTO{
					this->position,
					this->size,
					this->velocity,
					this->hexColor,
					this->image,
					this->srcRect
			});

		}

		virtual void onCollision(VisualElement* otherElement) = 0;
		virtual void update() = 0;
		bool checkCollision(VisualElement* otherElement) {

			Vector otherElementPosition = otherElement->getPosition();
			Vector otherElementSize = otherElement->getSize();

			if (this->position.x < otherElementPosition.x + otherElementSize.x &&
				this->position.x + this->size.x > otherElementPosition.x &&
				this->position.y < otherElementPosition.y + otherElementSize.y &&
				this->size.y + this->position.y > otherElementPosition.y) {
				return true;
			}
			return false;
		}

		void preventTranposition(VisualElement* otherElement) {
			Vector otherElementPosition = otherElement->getPosition();
			Vector otherElementSize = otherElement->getSize();

			float overlapLeft = (this->position.x + this->size.y) - otherElementPosition.x;
			float overlapRight = (otherElementPosition.x + otherElementSize.y) - this->position.x;
			float overlapTop = (this->position.y + this->size.x) - otherElementPosition.y;
			float overlapBottom = (otherElementPosition.y + otherElementSize.x) - this->position.y;

			float minOverlapX = std::min(overlapLeft, overlapRight);
			float minOverlapY = std::min(overlapTop, overlapBottom);

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

		Vector getPosition()
		{
			return this->position;
		}

		Vector getSize()
		{
			return this->size;
		}

		Vector getVelocity()
		{
			return this->velocity;
		}

		void setVelocity(Vector velocity)
		{
			this->velocity = velocity;
		}

		void setPosition(Vector position)
		{
			this->position = position;
		}

		void setSize(Vector size)
		{
			this->size = size;
		}

		void setHexColor(std::string hexColor)
		{
			this->hexColor = hexColor;
		}

		void physicsUpdate(float deltaTime)
		{
			this->position += this->velocity * deltaTime;
		}
		
		bool isDeleted() {
			return this->deleted;
		}

		void destroy() {
			this->deleted = true;
		}
		void setFrames(const std::vector<SDL_Rect>& newFrames);
		void updateAnimation();
	};
}
