#include "UserInterface.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_sdlrenderer.h"
#include <SDL.h>
#include <stdio.h>
#include "../GraphicsEngine.h"
#include <string>

//#define ADITIONAL_WINDOW		//< Tells if another window will be made for the UI.

UserInterface::UserInterface(GraphicsEngine* graphics) :
	mGraphics(graphics),
	mWindow(nullptr),
	mRenderer(nullptr)
{
	Init();
}

UserInterface::~UserInterface()
{
	Shutdown();
}

int UserInterface::Init()
{
#ifdef ADITIONAL_WINDOW
	// Setup Window
	SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	mWindow = SDL_CreateWindow(
		"Social Network Graph Controller",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		340,
		720,
		windowFlags
	);

	if (!mWindow)
	{
		SDL_Log("Unable to create UI Window: %s", SDL_GetError());
		return false;
	}

	// Setup renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
	);

	if (!mRenderer)
	{
		SDL_Log("Unable to create UI Renderer: %s", SDL_GetError());
		return false;
	}
#else	// Single Window Setup
	mWindow = mGraphics->GetWindow();
	mRenderer = mGraphics->GetRenderer();
#endif	// ADITIONAL_WINDOW

	// Setup IMGUI Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Enable Keyboard
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup IMGUI Window/Renderer
	ImGui_ImplSDL2_InitForSDLRenderer(mWindow);
	ImGui_ImplSDLRenderer_Init(mRenderer);

	return 0;
}

void UserInterface::Update()
{
	// Setup New Frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(mWindow);
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	// WINDOW EDITING CODE
	ImGui::Begin("Add Node");

	ImGui::End();

	// Rendering Code
	ImGui::Render();
	SDL_SetRenderDrawColor(mRenderer, 240, 240, 245, 255);
	SDL_RenderClear(mRenderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(mRenderer);
}


void UserInterface::Shutdown()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
}