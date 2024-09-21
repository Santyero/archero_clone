#include "SDLRendererAdapter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <SDL_image.h>

namespace Game {

    SDLRendererAdapter::SDLRendererAdapter(SDL_Window* sdlWindow) {
        this->sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        if (this->sdlRenderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(sdlWindow);
            SDL_Quit();
            throw std::runtime_error("Ocorreu um erro!");
        }
    }

    SDLRendererAdapter::~SDLRendererAdapter() {
        if (sdlRenderer) {
            SDL_DestroyRenderer(sdlRenderer);
        }
    }

    void SDLRendererAdapter::setRGBAColors(std::string_view hexColor) {
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

    /*void SDLRendererAdapter::renderPresent() {
        SDL_RenderPresent(this->sdlRenderer);
    }

    void SDLRendererAdapter::destroy() {
        if (sdlRenderer) {
            SDL_DestroyRenderer(sdlRenderer);
            sdlRenderer = nullptr;
        }
    }*/

} // namespace Game