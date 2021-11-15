#include "DynamicSpriteComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"
#include <iostream>

#define DYNAMIC_SPRITE_DEBUG

DynamicSpriteComponent::DynamicSpriteComponent(Actor* owner, int drawOrder) :
	SpriteComponent(owner, drawOrder),
	mDefaultSize(64),
	mNormalizeFactor(1)
{
	;
}

void DynamicSpriteComponent::AddTexture(std::string path)
{
	// Get Texture Width Size.
	int size;
	SDL_Texture* tex = mOwner->GetGraphicsEngine()->GetTexture(path);

	SDL_QueryTexture(tex, NULL, NULL, &size, nullptr);

#ifdef DYNAMIC_SPRITE_DEBUG
	std::cout << size << "\n";
#endif DYNAMIC_SPRITE_DEBUG

	if (!Math::IsPowerOfTwo(size))
	{
		std::cout << "ERROR (DynamicSpriteComponent -> AddTexture()). Could not add texture. Size is not power of two. Path: " << path << "\n";
		return;
	}

	mTexturesMap[size] = path;

	// Check if there is no texture
	if (!mTexture)
	{
		SetTexture(path);
		mCurrentSize = size;
		mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;
	}
}

void DynamicSpriteComponent::Draw(SDL_Renderer* renderer)
{
	SDL_Rect r;

	// STEP 1: Get Information

	Vector2D<float> relativePos = mOwner->GetPosition() - mOwner->GetGraphicsEngine()->GetCameraPos();
	Vector2D<int> screenCenterPos = mOwner->GetGraphicsEngine()->GetScreenCenterI();
	float zoom = mOwner->GetGraphicsEngine()->GetCameraZoom();

	// STEP 2: Calculate which texture size works best based on size (only width)
	float size = mTextureSize.x * mOwner->GetScale() * zoom * mNormalizeFactor;
	// if is bigger than current size and double the size exists in map
	if (size > mTextureSize.x + (mTextureSize.x / 2) && mTexturesMap[mTextureSize.x * 2] != "")
	{
		mCurrentSize *= 2;														// double the texture's size.
		SetTexture(mTexturesMap[mTextureSize.x * 2]);							// set new texture.
		mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;	// set new normalize factor.
	}
	else if (size < mTextureSize.x - (mTextureSize.x / 4) && mTexturesMap[mTextureSize.x / 2] != "")
	{
		mCurrentSize /= 2;														// double the texture's size.
		SetTexture(mTexturesMap[mTextureSize.x / 2]);							// set new texture.
		mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;	// set new normalize factor.
	}

	// STEP 3: Calculate Rect
	r.w = static_cast<int>(mTextureSize.x * mOwner->GetScale() * zoom * mNormalizeFactor);
	r.h = static_cast<int>(mTextureSize.y * mOwner->GetScale() * zoom * mNormalizeFactor);
	r.x = static_cast<int>(relativePos.x * zoom - r.w / 2 + screenCenterPos.x);
	r.y = static_cast<int>(relativePos.y * zoom - r.h / 2 + screenCenterPos.y);

#ifdef DYNAMIC_SPRITE_DEBUG
	std::cout << "Texture Rect Size: " << r.w << ", " << r.h << "\t";
	std::cout << "|Normalize Factor: " << mNormalizeFactor << "\t";
	std::cout << "|Current Path: " << mTexturesMap[mTextureSize.x] << "\n";
#endif

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