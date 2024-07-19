#pragma once
#include <SDL.h>
#include <iostream>
#include <string_view>
#include "RendererPort.h"
#include "RenderDataDTO.h"

namespace Game {
	class SDLRendererAdapter : public RendererPort {
	private:
		SDL_Renderer* sdlRenderer;
		Uint8 redColor = 0;
		Uint8 greenColor = 0;
		Uint8 blueColor = 0;
		Uint8 alphaColor = 0;

		void setRGBAColors(std::string_view hexColor);
	public:
		SDLRendererAdapter(SDL_Window* sdlWindow);

		virtual void renderElement(const RenderDataDTO& renderDataDTO) override;

		virtual void renderPresent() override {
			SDL_RenderPresent(this->sdlRenderer);
		}

		virtual void destroy() override {
			SDL_DestroyRenderer(this->sdlRenderer);
		}
	};
}