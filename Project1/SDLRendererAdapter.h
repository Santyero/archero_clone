#pragma once
#include <SDL.h>
#include <iostream>
#include <string_view>
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include <unordered_map>
#include <SDL_ttf.h>

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
		TTF_Font* font;

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

		//hub		
		void renderTexture(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) override;
		void setTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b) override;
		void setTextureAlphaMod(SDL_Texture* texture, Uint8 alpha) override;
		void renderSimpleText(const std::string& text, int x, int y, SDL_Color color) override;
	};
}