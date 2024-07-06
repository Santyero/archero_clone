#pragma once
#include "RendererPort.h"
#include "RenderDataDTO.h"

namespace Game
{
	class VisualElement
	{
	protected:
		RendererPort *rendererPort;
		float positionXInMeters;
		float positionYInMeters;
		float widthInMeters;
		float heightInMeters;
		std::string hexColor;
		bool deleted = false;

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

		virtual void onCollision(VisualElement* otherElement) = 0;
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

		bool isDeleted() {
			return this->deleted;
		}
	};
}
