#pragma once
#include "Component.h"
#include "../Math.h"
#include <string>
#include <SDL.h>

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	void SetTexture(std::string path);

private:
	int mDrawOrder;				//< The order in which sprites will be rendered.
	SDL_Texture* mTexture;	
	Vector2D<int> mTextureSize;	//< Texture size in pixels.
};