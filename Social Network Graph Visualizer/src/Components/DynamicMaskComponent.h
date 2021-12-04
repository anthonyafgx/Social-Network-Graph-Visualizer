#pragma once
#include "DynamicSpriteComponent.h"
#include "../Actors/Node.h"

class DynamicMaskComponent : public DynamicSpriteComponent
{
public:
	
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
	// mMasksMap[string COLOR][int TEXTURE SIZE] ex: mMasksMap[EColor::GREEN][64] = SDL_Texture*
	std::unordered_map<Node::EColor, std::unordered_map<int, SDL_Texture*> > mMasksMap;
};