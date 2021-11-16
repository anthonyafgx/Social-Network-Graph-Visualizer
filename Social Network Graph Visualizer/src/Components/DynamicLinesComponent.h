#pragma once
#include "SpriteComponent.h"

/*
* Draws the lines that correspond to Nodes Relations relative to camera
*/
class DynamicLinesComponent : public SpriteComponent
{
public:
	DynamicLinesComponent(class Node* owner, int drawOrder = 50);

	void Draw(SDL_Renderer* renderer) override;

private:
	
};