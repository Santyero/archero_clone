#pragma once
#include "RenderDataDTO.h";

namespace Game {
	class RendererPort
	{
	public:
		virtual void renderElement(const RenderDataDTO& renderDataDTO) = 0;
		virtual void renderPresent() = 0;
		virtual void destroy() = 0;
		virtual SDL_Renderer* getRenderer() = 0;
		virtual ~RendererPort() {}
		virtual void renderTexture(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) = 0;
		virtual void setTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b) = 0;
		virtual void setTextureAlphaMod(SDL_Texture* texture, Uint8 alpha) = 0;
		virtual void renderSimpleText(const std::string& text, int x, int y, SDL_Color color) = 0;
		virtual bool getCharPixel(char c, int x, int y) = 0;
	};
}
