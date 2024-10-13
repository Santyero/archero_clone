#include "SDLRendererAdapter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <SDL_image.h>


namespace Game {

    SDLRendererAdapter::SDLRendererAdapter(SDL_Window* sdlWindow) {
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!sdlRenderer) {
            throw std::runtime_error("Failed to create SDL renderer");
            SDL_DestroyWindow(sdlWindow);
            SDL_Quit();
            throw std::runtime_error("Ocorreu um erro!");
        }

        if (TTF_Init() == -1) {
            throw std::runtime_error("SDL_ttf could not initialize");
        }

        font = TTF_OpenFont("font.ttf", 24);
        if (!font) {
            throw std::runtime_error("Failed to load font");
        }
    }

    SDLRendererAdapter::~SDLRendererAdapter() {
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(sdlRenderer);
    }

    void SDLRendererAdapter::setRGBAColors(std::string hexColor) {
        std::string hex = std::string(hexColor);
        if (!hex.empty() && hex[0] == '#') {
            hex = hex.substr(1);
        }

        std::stringstream ss;
        ss << std::hex << hex;
        unsigned int hexValue;
        ss >> hexValue;

        this->redColor = (hexValue >> 16) & 0xFF;
        this->greenColor = (hexValue >> 8) & 0xFF;
        this->blueColor = hexValue & 0xFF;
        this->alphaColor = 255;

        if (hex.length() > 6) {
            this->alphaColor = (hexValue >> 24) & 0xFF;
        }
    }

    void SDLRendererAdapter::renderElement(const RenderDataDTO& renderDataDTO) {
        SDL_Rect destRect = {
            static_cast<int>(renderDataDTO.position.x),
            static_cast<int>(renderDataDTO.position.y),
            static_cast<int>(renderDataDTO.size.x),
            static_cast<int>(renderDataDTO.size.y)
        };

        if (renderDataDTO.texture != nullptr) {
            SDL_RendererFlip flip = renderDataDTO.flipHorizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(this->sdlRenderer, renderDataDTO.texture, nullptr, &destRect, 0, NULL, flip);
        }
        else {
            this->setRGBAColors(renderDataDTO.hexColor);
            SDL_SetRenderDrawColor(this->sdlRenderer, this->redColor, this->greenColor, this->blueColor, this->alphaColor);
            SDL_RenderFillRect(this->sdlRenderer, &destRect);
        }
    }

    void SDLRendererAdapter::renderHealthBar(const Vector& position, const Vector& size, float healthPercentage) {
        // Barra de fundo (vermelha)
        SDL_Rect backgroundRect = {
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(size.x),
            static_cast<int>(size.y)
        };
        SDL_SetRenderDrawColor(this->sdlRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(this->sdlRenderer, &backgroundRect);

        // Barra de vida atual (verde)
        SDL_Rect healthRect = {
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(size.x * healthPercentage),
            static_cast<int>(size.y)
        };
        SDL_SetRenderDrawColor(this->sdlRenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(this->sdlRenderer, &healthRect);
    }
    
    void SDLRendererAdapter::renderTexture(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) {
        SDL_RenderCopy(this->sdlRenderer, texture, srcrect, dstrect);
    }

    void SDLRendererAdapter::setTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b) {
        SDL_SetTextureColorMod(texture, r, g, b);
    }

    void SDLRendererAdapter::setTextureAlphaMod(SDL_Texture* texture, Uint8 alpha) {
        SDL_SetTextureAlphaMod(texture, alpha);
    }

    void SDLRendererAdapter::renderSimpleText(const std::string& text, int x, int y, SDL_Color color) {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surfaceMessage) {
            std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* message = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage);
        if (!message) {
            std::cerr << "Failed to create texture from rendered text: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        SDL_Rect messageRect;
        messageRect.x = x;
        messageRect.y = y;
        SDL_QueryTexture(message, NULL, NULL, &messageRect.w, &messageRect.h);
        SDL_RenderCopy(sdlRenderer, message, NULL, &messageRect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(message);
    }

} // namespace Game