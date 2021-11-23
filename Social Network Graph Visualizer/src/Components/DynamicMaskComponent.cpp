#include "DynamicMaskComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"
#include <iostream>

DynamicMaskComponent::DynamicMaskComponent(Actor* owner, int drawOrder) :
	DynamicSpriteComponent(owner, drawOrder)
{
	// MODIFIABLE VALUES
	// colors (modifiable)
	const RGB DEFAULT = { 153, 195, 250 };
	const RGB LEFTCLICK = { 250, 207, 127 };

	// available texture sizes (modifiable)
	std::vector<int> SIZES = { 32, 64, 128, 256, 512 };

	
	// Initialize array for each color with all texture sizes
	std::string defaultPath;
	std::string leftclickPath;
	for (auto itr = SIZES.begin(); itr != SIZES.end(); ++itr)
	{
		// RECOLOR DEFAULT
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_a.png";								// get path
		mMasksMap["DEFAULT"][*(itr)] = GetTexture(defaultPath);											// add to map
		SDL_SetTextureColorMod(mMasksMap["DEFAULT"][*(itr)], DEFAULT.r, DEFAULT.g, DEFAULT.b);			// recolor tex

		// RECOLOR LEFTCLICK
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_b.png";								// get path
		mMasksMap["LEFTCLICK"][*(itr)] = GetTexture(defaultPath);										// add to map
		SDL_SetTextureColorMod(mMasksMap["LEFTCLICK"][*(itr)], LEFTCLICK.r, LEFTCLICK.g, LEFTCLICK.b);	// recolor tex
	}

	// Set default tex
	SetTexture(mMasksMap["DEFAULT"][64]);
}

void DynamicMaskComponent::Draw(SDL_Renderer* renderer)
{
	if (!mTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot draw sprite, texture not found");
		return;
	}


	// Choose Color & Corresponding Tex
	const int texSize = mOwner->GetDynamicTexSize();
	const Actor::Behavior behavior = mOwner->GetBehavior();
	if (behavior == Actor::MouseLeftClick)
	{
		SetTexture(mMasksMap["LEFTCLICK"][texSize]);
	}
	else if (behavior == Actor::None)
	{
		SetTexture(mMasksMap["DEFAULT"][texSize]);
	}

	// Draw
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	//< Blendmode modifiable.
	SDL_RenderCopyEx(
		renderer,
		mTexture,
		nullptr,
		&mOwner->GetRelativeRect(),
		-Math::ToDegrees(mOwner->GetRotation()),
		nullptr,
		SDL_FLIP_NONE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}