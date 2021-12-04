#pragma once
#include <SDL.h>

class UserInterface
{
public:
	UserInterface(class GraphicsEngine* graphics);
	~UserInterface();

	void Update();

	// Getters
	SDL_Window* GetWindow() { return mWindow; }

private:
	int Init();
	void Shutdown();

private:
	class GraphicsEngine* mGraphics;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
};