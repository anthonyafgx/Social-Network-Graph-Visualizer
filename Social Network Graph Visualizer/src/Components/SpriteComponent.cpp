#include "SpriteComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"
#include <iostream>

#define DEBUG_SPRITE

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) : 
	Component(owner), 
	mDrawOrder(drawOrder)
{
	// Add sprite to graphics engine class
	mOwner->GetGraphicsEngine()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	// Remove sprite from graphics engine class
	mOwner->GetGraphicsEngine()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	// texture not found
	if (!mTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot draw sprite, texture not found");
		return;
	}

	// Create Rect
	SDL_Rect r;

	r.w = static_cast<int>(mTextureSize.x * mOwner->GetScale());
	r.h = static_cast<int>(mTextureSize.y * mOwner->GetScale());
	r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
	r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

	mOwner->SetRelativeRect(r);

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderFillRect(renderer, &r);

	// Render
	SDL_RenderCopyEx(
		renderer,										//< renderer
		mTexture,										//< texture
		nullptr,										//< source rect (which part of tex will be drawn)
		&r,												//< dest rect (where will tex be drawn)
		-Math::ToDegrees(GetOwner()->GetRotation()),	//< rotation in degrees
		nullptr,										//< point from which texture will be rotated (NULL if center)
		SDL_FLIP_NONE									//< flip
	);
}

SDL_Texture* SpriteComponent::GetTexture(std::string path, Uint8 alpha)
{
	SDL_Texture* tex = mOwner->GetGraphicsEngine()->GetTexture(path, alpha);

	// if no texture was found
	if (!tex)
	{
		SDL_Log("No texture was found");
		return nullptr;
	}

	return tex;
}

void SpriteComponent::SetTexture(std::string path, Uint8 alpha)
{
	SDL_Texture* tex = GetTexture(path, alpha);

	SetTexture(tex);
};

void SpriteComponent::SetTexture(SDL_Texture* tex)
{
	if (!tex)
	{
		SDL_Log("No texture was found");
	}

	mTexture = tex;

	// Get texture's size (in pixels)
	SDL_QueryTexture(tex, NULL, NULL, &mTextureSize.x, &mTextureSize.y);
}