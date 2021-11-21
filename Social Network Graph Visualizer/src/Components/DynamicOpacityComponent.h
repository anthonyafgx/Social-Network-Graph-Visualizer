#pragma once
#include "DynamicSpriteComponent.h"
/*
* Draws a white transpartent circle on top of a DynamicSpriteComponent when
* mouse is hovering over it.
*
* IMPORTANT: USED WITH DYNAMIC SPRITE COMPONENT*
* Texutures used should be the same as the DynamicSpriteComponent but singlecolored
* White for example.
*/
class DynamicOpacityComponent : public DynamicSpriteComponent
{
public:
	DynamicOpacityComponent(class Actor* owner, int drawOrder = 150);

	void Draw(SDL_Renderer* renderer) override;
	// The last number is adjustable (alpha value).
	void AddTexture(std::string path, Uint8 alpha = 255) override { DynamicSpriteComponent::AddTexture(path, 125); }
};