#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "Math.h"

class GraphicsEngine
{
public:
	GraphicsEngine();

	// GraphicsEngine Loop Functions
	/*
	* Initializes the GraphicsEngine Engine
	* @return Bool Value indicating success
	*/
	bool Initialize();
	void RunLoop();
	void Shutdown();

	// GraphicsEngine Functions
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	//void AddNode(class Node* node);
	//void RemoveNode(class Node* node);

	// Getters / Setters
	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	SDL_Texture* GetTexture(std::string path);

	const Vector2D<int> GetScreenCenterI() { return Vector2D<int>(mWindowWidth / 2, mWindowHeight / 2); }
	const Vector2D<float> GetScreenCenterF() { return Vector2D<float>(mWindowWidth / 2, mWindowHeight / 2); }

	const Vector2D<float> GetCameraPos();
	float GetCameraZoom();

private:
	// Loop Related Methods
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Initialize / Shutdown Related Methods
	void LoadData();
	void UnloadData();

private:
	// Containers
	std::vector<class Actor*> mActors;							//!< Vector containing pointers to each Actor
	std::vector<class Actor*> mPendingActors;					//!< Vector containing pointers to Actors that will be moved to mActors
	std::vector<class SpriteComponent*> mSprites;				//!< Vector containing pointers to each Sprite (sorted by drawing order)
	std::unordered_map<std::string, SDL_Texture*> mTextures;	//!< key = path ; value = texture pointer

	// GraphicsEngine info
	bool mIsRunning;											//!< Indicates if game should continue run
	bool mUpdatingActors;										//!< Indicates if actors are currently being updated

	// SDL
	SDL_Window* mWindow;										//!< SDL Window pointer
	SDL_Renderer* mRenderer;									//!< SDL Renderer pointer
	Uint32 mTicksCount;											//!< SDL Ticks Count in milliseconds
	const int mWindowWidth;
	const int mWindowHeight;

	// Program Specific
	class Actor* mCamera;
	class Graph* mGraph;
};

