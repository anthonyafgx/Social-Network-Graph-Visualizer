#pragma once
#include "DynamicSpriteComponent.h"

class DynamicMaskComponent : public DynamicSpriteComponent
{
public:
	DynamicMaskComponent(class Actor* owner, int drawOrder = 125);

	void Draw(SDL_Renderer* renderer) override;

private:
	struct RGB
	{
		int r, g, b;
		bool equals(RGB col)
		{
			return (r == col.r) && (g == col.g) && (b == col.b);
		}
	};

private:
	// mMasksMap[string SITUATION][int TEXTURE SIZE] ex: mMasksMap["LEFTCLICK"][64] = SDL_Texture*
	std::unordered_map<std::string, std::unordered_map<int, SDL_Texture*> > mMasksMap;
};