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
		float positionXInMeters;
		float positionYInMeters;
		float widthInMeters;
		float heightInMeters;
		std::string hexColor;
		Vector position;
		Vector velocity;

	public:
		VisualElement(RendererPort *adapter, const RenderDataDTO &renderDataDTOParam);

		void renderElement()
		{
			this->rendererPort->renderElement(RenderDataDTO{
					this->positionXInMeters,
					this->positionYInMeters,
					this->widthInMeters,
					this->heightInMeters,
					this->hexColor});
		}

		virtual void onCollision() = 0;
		virtual void update() = 0;
		void checkCollision(VisualElement *otherElement);

		float getPositionXInMeters()
		{
			return this->positionXInMeters;
		}

		float getPositionYInMeters()
		{
			return this->positionYInMeters;
		}

		void physicsUpdate(float deltaTime)
		{
			this->position += this->velocity * deltaTime;
		}
	};
}
