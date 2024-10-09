#include <SDL.h>
#include <iostream>

#include "Window.h"
#include "Player.h"
#include "GameEngine.h"
#include "config.h"
#include "SDLTimeServiceAdapter.h"
#include "SDLRendererAdapter.h"
#include <SDL_image.h>

using namespace Game;


int main(int argc, char* argv[]) {
    Window window = Window(Config::windowSize.x, Config::windowSize.y);
    window.createWindow();
    SDLRendererAdapter* sdlRendererAdapter = new SDLRendererAdapter(window.getWindowRef());
    SDLTimeServiceAdapter* sdlTimeServiceAdapter = new SDLTimeServiceAdapter();
    
    GameEngine game = GameEngine(window, sdlRendererAdapter, sdlTimeServiceAdapter);
    game.run();
    SDL_DestroyWindow(window.getWindowRef());
    SDL_Quit();
    return 0;
}