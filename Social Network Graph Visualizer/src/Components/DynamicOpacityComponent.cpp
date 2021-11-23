#include "DynamicOpacityComponent.h"
#include "../GraphicsEngine.h"
#include "../Actors/Node.h"

//#define DYNAMIC_OPACITY_DEBUG

DynamicOpacityComponent::DynamicOpacityComponent(Actor* owner, int drawOrder) :
	DynamicSpriteComponent(owner, drawOrder)
{

}

void DynamicOpacityComponent::SetColor(int r, int g, int b)
{
	// Colors can't be greater than 255.
	mR = (r > 255) ? 255 : r;
	mG = (g > 255) ? 255 : g;
	mB = (b > 255) ? 255 : b;

	// Colors can't be negative
	mR = (r < 0) ? 0 : r;
	mG = (g < 0) ? 0 : g;
	mB = (b < 0) ? 0 : b;
}

void DynamicOpacityComponent::Draw(SDL_Renderer* renderer)
{
	/* DRAW WHITE WHEN MOUSE ON TOP */
	// Get Actor's Behavior respect to mouse
	const Actor::Behavior behavior = mOwner->GetBehavior();

	if (behavior == Actor::Behavior::MouseOnTop)
	{
		// texture not found
		if (!mTexture)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot draw sprite, texture not found");
			return;
		}

		// STEP 1: Get Information
		float zoom = mOwner->GetGraphicsEngine()->GetCameraZoom();

		// STEP 2: Calculate which texture size works best based on size (only width)
		float size = mTextureSize.x * mOwner->GetScale() * zoom * mNormalizeFactor;
		// if is bigger than current size and double the size exists in map
		if (size > mTextureSize.x + (mTextureSize.x / 2) && mTexturesMap[mTextureSize.x * 2] != "")
		{
			SetTexture(mTexturesMap[mTextureSize.x * 2]);							// set new texture.
			mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;	// set new normalize factor.
		}
		else if (size < mTextureSize.x - (mTextureSize.x / 4) && mTexturesMap[mTextureSize.x / 2] != "")
		{
			SetTexture(mTexturesMap[mTextureSize.x / 2]);							// set new texture.
			mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;	// set new normalize factor.
		}

#ifdef DYNAMIC_OPACITY_DEBUG
		std::cout << "Texture Rect Size: " << width << ", " << height << "\t";
		std::cout << "|Normalize Factor: " << mNormalizeFactor << "\t";
		std::cout << "|Current Path: " << mTexturesMap[mTextureSize.x] << "\n";
#endif

		// Render
		SDL_RenderCopyEx(
			renderer,										//< renderer
			mTexture,										//< texture
			nullptr,										//< source rect (which part of tex will be drawn)
			&mOwner->GetRelativeRect(), 					//< dest rect (where will tex be drawn)
			-Math::ToDegrees(GetOwner()->GetRotation()),	//< rotation in degrees
			nullptr,										//< point from which texture will be rotated (NULL if center)
			SDL_FLIP_NONE									//< flip
		);
	}
}