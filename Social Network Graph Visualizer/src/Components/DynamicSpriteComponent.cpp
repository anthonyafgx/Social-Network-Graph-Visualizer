#include "DynamicSpriteComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"
#include <iostream>

//#define DYNAMIC_SPRITE_DEBUG

DynamicSpriteComponent::DynamicSpriteComponent(Actor* owner, int drawOrder) :
	SpriteComponent(owner, drawOrder),
	mDefaultSize(64),
	mNormalizeFactor(1)
{
	;
}

void DynamicSpriteComponent::AddTexture(std::string path, Uint8 alpha)
{
	// Get Texture Width Size.
	int size;
	SDL_Texture* tex = mOwner->GetGraphicsEngine()->GetTexture(path, alpha);

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
		mNormalizeFactor = static_cast<float>(mDefaultSize) / mTextureSize.x;
	}
}

void DynamicSpriteComponent::Draw(SDL_Renderer* renderer)
{
	// texture not found
	if (!mTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot draw sprite, texture not found");
		return;
	}

	// STEP 1: Get Information

	Vector2D<float> relativePos = mOwner->GetPosition() - mOwner->GetGraphicsEngine()->GetCameraPos();
	Vector2D<int> screenCenterPos = mOwner->GetGraphicsEngine()->GetScreenCenterI();
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

	// STEP 3: Calculate Rect
	int width = static_cast<int>(mTextureSize.x * mOwner->GetScale() * zoom * mNormalizeFactor);
	int height = static_cast<int>(mTextureSize.y * mOwner->GetScale() * zoom * mNormalizeFactor);
	int x = static_cast<int>(relativePos.x * zoom - width / 2 + screenCenterPos.x);
	int y = static_cast<int>(relativePos.y * zoom - height / 2 + screenCenterPos.y);

	// STEP 4: Update Actor's Rect.
	mOwner->SetRelativeRect(SDL_Rect{ x, y, width, height });
	mOwner->SetDynamicTexSize(mTextureSize.x);

#ifdef DYNAMIC_SPRITE_DEBUG
	std::cout << "Texture Rect Size: " << width << ", " << height << "\t";
	std::cout << "|Normalize Factor: " << mNormalizeFactor << "\t";
	std::cout << "|Current Path: " << mTexturesMap[mTextureSize.x] << "\n";
#endif

	// STEP 5: Render
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