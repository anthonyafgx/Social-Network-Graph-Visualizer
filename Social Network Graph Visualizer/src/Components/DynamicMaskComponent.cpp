#include "DynamicMaskComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"
#include <iostream>
#include "../Actors/Node.h"

DynamicMaskComponent::DynamicMaskComponent(Actor* owner, int drawOrder) :
	DynamicSpriteComponent(owner, drawOrder)
{
	// MODIFIABLE VALUES
	// colors (modifiable)
	const RGB DEFAULT = { 153, 195, 250 };
	const RGB YELLOW = { 250, 207, 127 };
	const RGB GREEN = { 102, 250, 172 };
	const RGB RED = { 250, 151, 140 };

	// available texture sizes (modifiable)
	std::vector<int> SIZES = { 32, 64, 128, 256, 512 };

	
	// Initialize array for each color with all texture sizes
	std::string defaultPath;
	std::string leftclickPath;
	for (auto itr = SIZES.begin(); itr != SIZES.end(); ++itr)
	{
		// RECOLOR DEFAULT
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_a.png";									// get path
		mMasksMap[Node::EColor::DEFAULT][*(itr)] = GetTexture(defaultPath);									// add to map
		SDL_SetTextureColorMod(mMasksMap[Node::EColor::DEFAULT][*(itr)], DEFAULT.r, DEFAULT.g, DEFAULT.b);	// recolor tex

		// RECOLOR GREEN
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_b.png";									// get path
		mMasksMap[Node::EColor::GREEN][*(itr)] = GetTexture(defaultPath);									// add to map
		SDL_SetTextureColorMod(mMasksMap[Node::EColor::GREEN][*(itr)], GREEN.r, GREEN.g, GREEN.b);			// recolor tex

		// RECOLOR RED
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_c.png";									// get path
		mMasksMap[Node::EColor::RED][*(itr)] = GetTexture(defaultPath);										// add to map
		SDL_SetTextureColorMod(mMasksMap[Node::EColor::RED][*(itr)], RED.r, RED.g, RED.b);					// recolor tex

		// RECOLOR YELLOW
		defaultPath = "Assets/x" + std::to_string(*(itr)) + "/mask_d.png";									// get path
		mMasksMap[Node::EColor::YELLOW][*(itr)] = GetTexture(defaultPath);									// add to map
		SDL_SetTextureColorMod(mMasksMap[Node::EColor::YELLOW][*(itr)], YELLOW.r, YELLOW.g, YELLOW.b);				// recolor tex
	}

	// Set default tex
	SetTexture(mMasksMap[Node::EColor::DEFAULT][64]);
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
	const Actor::EColor color = mOwner->GetColor();

	if (mTexture != mMasksMap[color][texSize])
	{
		SetTexture(mMasksMap[color][texSize]);
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