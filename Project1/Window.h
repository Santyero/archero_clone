#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

namespace Game {
	class Window
	{
	private:
		SDL_Window* windowRef = nullptr;
		int width;
		int height;
		bool isFullscreen;
	public:
		Window(int width, int height);

		void createWindow();
		void toggleFullscreen();
		bool getIsFullscreen() const { return isFullscreen; }
		int getWidth() const { return width; }
		int getHeight() const { return height; }

		SDL_Window* getWindowRef();
	};
}
#endif // WINDOW_H
