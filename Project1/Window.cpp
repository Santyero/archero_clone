#include "Window.h"
#include <SDL.h>
#include <iostream>

using namespace Game;

Window::Window(int width, int height) {
    this->width = width;
    this->height = height;
}

void Window::createWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Ocorreu um erro!");
    }

    this->windowRef = SDL_CreateWindow(
        "Moving Square", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN
    );
    if (this->windowRef == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw std::runtime_error("Ocorreu um erro!");
    }
}

SDL_Window* Window::getWindowRef() {
    return this->windowRef;
}

void Window::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    if (isFullscreen) {
        SDL_SetWindowFullscreen(windowRef, SDL_WINDOW_FULLSCREEN_DESKTOP);

        // Obtenha o tamanho da tela
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);

        // Calcule a escala para manter a proporção
        float scaleX = static_cast<float>(DM.w) / width;
        float scaleY = static_cast<float>(DM.h) / height;
        float scale = std::min(scaleX, scaleY);

        // Defina o viewport
        int viewportWidth = static_cast<int>(width * scale);
        int viewportHeight = static_cast<int>(height * scale);
        int viewportX = (DM.w - viewportWidth) / 2;
        int viewportY = (DM.h - viewportHeight) / 2;
        SDL_Rect viewport = { viewportX, viewportY, viewportWidth, viewportHeight };
        SDL_RenderSetViewport(SDL_GetRenderer(windowRef), &viewport);
    }
    else {
        SDL_SetWindowFullscreen(windowRef, 0);
        SDL_RenderSetViewport(SDL_GetRenderer(windowRef), NULL);
    }
}