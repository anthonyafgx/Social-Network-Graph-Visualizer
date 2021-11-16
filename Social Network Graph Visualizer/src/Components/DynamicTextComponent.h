#pragma once
#include "SpriteComponent.h"

class DynamicTextComponent : public SpriteComponent
{
public:
	DynamicTextComponent(class Actor* owner, int drawOrder = 200);

	void Draw(SDL_Renderer* renderer) override;

	void SetText(std::string text);
	void SetColor(SDL_Color color) { mColor = color; }

private:
	SDL_Color mColor;
};