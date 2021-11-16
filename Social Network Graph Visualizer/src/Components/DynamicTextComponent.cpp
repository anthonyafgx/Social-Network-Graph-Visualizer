#include "DynamicTextComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"

DynamicTextComponent::DynamicTextComponent(Actor* owner, int drawOrder) :
	SpriteComponent(owner, drawOrder),
	mColor({ 0xFF,0xFF,0xFF,0xFF })
{

}

void DynamicTextComponent::SetText(std::string text)
{
	SDL_Texture* tex = mOwner->GetGraphicsEngine()->GetTextureFromText(text, mColor);

	if (!tex)
	{
		return;
	}

	SDL_QueryTexture(tex, NULL, NULL, &mTextureSize.x, &mTextureSize.y);
	
	mTexture = tex;
}

void DynamicTextComponent::Draw(SDL_Renderer* renderer)
{
	// texture not found
	if (!mTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot draw sprite, texture not found");
		return;
	}

	SDL_Rect r;

	// STEP 1: Get Information
	Vector2D<float> relativePos = mOwner->GetPosition() - mOwner->GetGraphicsEngine()->GetCameraPos();
	Vector2D<int> screenCenterPos = mOwner->GetGraphicsEngine()->GetScreenCenterI();
	float zoom = mOwner->GetGraphicsEngine()->GetCameraZoom();

	// STEP 2: Calculate Rect
	r.w = static_cast<int>(mTextureSize.x * mOwner->GetScale() * zoom);
	r.h = static_cast<int>(mTextureSize.y * mOwner->GetScale() * zoom);
	r.x = static_cast<int>(relativePos.x * zoom - r.w / 2 + screenCenterPos.x);
	r.y = static_cast<int>(relativePos.y * zoom - r.h / 2 + screenCenterPos.y);

	// STEP 3: Render
	SDL_RenderCopyEx(
		renderer,
		mTexture,
		nullptr,
		&r,
		-Math::ToDegrees(mOwner->GetRotation()),
		nullptr,
		SDL_FLIP_NONE
	);
}