#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

Game::Game() : 
	mIsRunning(true), 
	mWindow(nullptr), 
	mWindowWidth(1280), 
	mWindowHeight(720) { ; }

// Game Loop Functions

bool Game::Initialize()
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
		"Dev Game - anthonyafgx studios",
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

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();

	IMG_Quit();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

// Game Process Functions

void Game::AddActor(Actor* actor)
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

void Game::RemoveActor(Actor* actor)
{
	auto itr = std::find(mActors.begin(), mActors.end(), actor);
	if (itr != mActors.end())
	{
		mActors.erase(itr);
	}
}

void Game::AddSprite(SpriteComponent* sprite)
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

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto itr = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (itr != mSprites.end())
	{
		mSprites.erase(itr);
	}
}

// Getters / Setters

SDL_Texture* Game::GetTexture(std::string path)
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

// Loop Related Methods

void Game::ProcessInput()
{
	;
}

void Game::UpdateGame()
{
	;
}

void Game::GenerateOutput()
{
	;
}

// Initialize / Shutdown Related Methods
void Game::LoadData()
{
	;
}

void Game::UnloadData()
{
	;
}