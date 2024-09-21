#pragma once
#include <SDL.h>
#include <iostream>
#include <string_view>
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include <unordered_map>

namespace Game {
	class SDLRendererAdapter : public RendererPort {
	private:
		SDL_Renderer* sdlRenderer;
		Uint8 redColor = 0;
		Uint8 greenColor = 0;
		Uint8 blueColor = 0;
		Uint8 alphaColor = 0;
		std::unordered_map<int, SDL_Texture*> textures;
		int nextTextureId;

		void setRGBAColors(std::string_view hexColor);
	public:
		SDLRendererAdapter(SDL_Window* sdlWindow);
		virtual ~SDLRendererAdapter();

		virtual void renderElement(const RenderDataDTO& renderDataDTO) override;

		virtual void renderPresent() override {
			SDL_RenderPresent(this->sdlRenderer);
		}

		virtual void destroy() override {
			SDL_DestroyRenderer(this->sdlRenderer);
		}

		SDL_Renderer* getRenderer() override {
			return this->sdlRenderer;
		}

		void renderHealthBar(const Vector& position, const Vector& size, float healthPercentage);
	};
}