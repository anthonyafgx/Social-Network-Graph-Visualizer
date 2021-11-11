#include "GraphicsEngine.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Components/SpriteComponent.h"
#include "Actors/Actor.h"
#include "Actors/Node.h"
#include "Actors/Camera.h"

#define FPS_CAP

GraphicsEngine::GraphicsEngine() : 
	mIsRunning(true), 
	mWindow(nullptr), 
	mWindowWidth(1280), 
	mWindowHeight(720) { ; }

// GraphicsEngine Loop Functions

bool GraphicsEngine::Initialize()
{
	// Initialization
	int init = SDL_Init(SDL_INIT_VIDEO);

	if (init != 0)
	{
		SDL_Log("Unable to initialize: %s", SDL_GetError());
		return false;
	}

	// Create Window
	mWindow = SDL_CreateWindow(
		"Dev GraphicsEngine - anthonyafgx studios",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		mWindowWidth,
		mWindowHeight,
		0
	);

	if (!mWindow)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	// Create Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,														//< index: Default driver
		SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC	//< flags: Hardware & VSync
	);

	if (!mRenderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
	}

	// SDL Image
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	int imgInit = IMG_Init(imgFlags);

	if ((imgInit&imgFlags) != imgFlags)
	{
		SDL_Log("Unable to initialize IMAGE support: %s\n", IMG_GetError());
		system("pause");
		mIsRunning = false;
	}

	LoadData();

	return true;
}

void GraphicsEngine::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void GraphicsEngine::Shutdown()
{
	UnloadData();

	IMG_Quit();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

// GraphicsEngine Process Functions

void GraphicsEngine::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{	// add to queue
		mPendingActors.emplace_back(actor);
	}
	else
	{	// add directly
		mActors.emplace_back(actor);
	}
}

void GraphicsEngine::RemoveActor(Actor* actor)
{
	auto itr = std::find(mActors.begin(), mActors.end(), actor);
	if (itr != mActors.end())
	{
		mActors.erase(itr);
	}
}

void GraphicsEngine::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();

	auto itr = mSprites.begin();

	for (; itr != mSprites.end(); itr++)
	{
		if ((*itr)->GetDrawOrder() >= myDrawOrder)
		{
			break;
		}
	}
	mSprites.insert(itr, sprite);
}

void GraphicsEngine::RemoveSprite(SpriteComponent* sprite)
{
	auto itr = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (itr != mSprites.end())
	{
		mSprites.erase(itr);
	}
}

// Getters / Setters

SDL_Texture* GraphicsEngine::GetTexture(std::string path)
{
	// Pointer that will be returned
	SDL_Texture* tex = nullptr;

	// Check if file exists
	struct stat buffer;
	bool exists = (stat(path.c_str(), &buffer) == 0);

	if (!exists)
	{
		printf("ERROR: Could not find %s\n", path.c_str());
		// handle error
		system("pause");
		mIsRunning = false;
		return nullptr;
	}

	auto itr = mTextures.find(path);

	// if found, return pointer directly
	if (itr != mTextures.end())
	{
		return itr->second;
	}
	else
	{
		// Load path image to surface
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
		{
			// unable to load image
			SDL_Log("Unable to load image: %s\n", path.c_str(), IMG_GetError());
			// handle error
		}

		// Convert surface to texture
		tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (!tex)
		{
			SDL_Log("Unable to convert surface to texture '%s' due to: %s\n", path.c_str(), SDL_GetError());
			// handle error
			system("pause");
			mIsRunning = false;
		}

		// Delete Surface
		SDL_FreeSurface(surface);

		// Add tex to Map
		mTextures.emplace(path, tex);
	}

	// Return tex
	return tex;
}

const Vector2D<float> GraphicsEngine::GetCameraPos()
{
	return mCamera->GetPosition();
}

float GraphicsEngine::GetCameraZoom()
{
	return mCamera->GetZoomScale();
}

// Loop Related Methods

void GraphicsEngine::ProcessInput()
{
	// Handle Events
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	// Process Input

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	for (Actor* actor : mActors)
	{
		actor->ProcessInput(keyboardState);
	}
}

void GraphicsEngine::UpdateGame()
{
#ifdef FPS_CAP
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	{
		;
	}
#endif

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	// Delta Time Cap (30fps)
	if (deltaTime >= 0.033f)
	{
		deltaTime = 0.033f;
	}

	// Update Actors
	mUpdatingActors = true;
	for (Actor* actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move Pending Actors to mActors
	for (Actor* actor : mPendingActors)
	{
		mActors.emplace_back(actor);
	}
	mPendingActors.clear();
}

void GraphicsEngine::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 200, 255);
	SDL_RenderClear(mRenderer);

	// Render Sprites
	for (SpriteComponent* sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

// Initialize / Shutdown Related Methods
void GraphicsEngine::LoadData()
{
	new Node(this);
	
	mCamera = new Camera(this);
}

void GraphicsEngine::UnloadData()
{
	// Delete Actors
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Delete pending actors
	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}

	// Destroy textures
	for (auto tex : mTextures)
	{
		SDL_DestroyTexture(tex.second);
	}
}