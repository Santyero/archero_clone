#include "SDLRendererAdapter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>

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
        SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);

        int charWidth = 8;
        int charHeight = 12;
        int spacing = 2;

        for (char c : text) {
            for (int dy = 0; dy < charHeight; dy++) {
                for (int dx = 0; dx < charWidth; dx++) {
                    if (getCharPixel(c, dx, dy)) {
                        SDL_RenderDrawPoint(sdlRenderer, x + dx, y + dy);
                    }
                }
            }
            x += charWidth + spacing;
        }
    }

    bool SDLRendererAdapter::getCharPixel(char c, int x, int y) {
        // Defina aqui a forma dos caracteres (pode ser um mapa de bits para cada caractere)
        // Este é um exemplo simples para o número '1'
        static const char* one[] = {
            "  X ",
            " XX ",
            "  X ",
            "  X ",
            " XXX"
        };

        if (c == '1' && y < 5 && x < 4) {
            return one[y][x] == 'X';
        }

        // Adicione mais caracteres conforme necessário

        return false;
    }

} // namespace Game