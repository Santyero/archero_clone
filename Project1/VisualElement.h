#pragma once
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include "math-vector.h"

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

	public:
		VisualElement(RendererPort *adapter, const RenderDataDTO &renderDataDTOParam);

		void renderElement()
		{
			this->rendererPort->renderElement(RenderDataDTO{
					this->position,
					this->size,
					this->velocity,
					this->hexColor
			});
		}

		virtual void onCollision(VisualElement* otherElement) = 0;
		virtual void update() = 0;
		virtual void checkCollision(VisualElement *otherElement) = 0;

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
	};
}
