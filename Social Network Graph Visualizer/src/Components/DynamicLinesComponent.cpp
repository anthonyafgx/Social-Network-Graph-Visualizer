#include "DynamicLinesComponent.h"
#include "../GraphicsEngine.h"
#include "../Actors/Node.h"

DynamicLinesComponent::DynamicLinesComponent(Node* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder),
	mBlack(SDL_Color{ 100, 100, 100, 255 }),
	mHighlight(mBlack) // < Currently disabled
	//mHighlight(SDL_Color{ 250, 151, 140, 255 }) 
{

}

void DynamicLinesComponent::Draw(SDL_Renderer* renderer)
{
	// Get Information
	Vector2D<float> relativePos = mOwner->GetPosition() - mOwner->GetGraphicsEngine()->GetCameraPos();
	Vector2D<int> screenCenterPos = mOwner->GetGraphicsEngine()->GetScreenCenterI();
	float zoom = mOwner->GetGraphicsEngine()->GetCameraZoom();

	float x1 = relativePos.x * zoom + screenCenterPos.x;
	float y1 = relativePos.y * zoom + screenCenterPos.y;

	for (auto& adjacent : mOwner->GetAdjacentNodes())
	{
		Vector2D<float> adjacentRelativePos = adjacent->GetPosition() - adjacent->GetGraphicsEngine()->GetCameraPos();

		// Determine color if being highlighted
		Node::EColor ownerColor = mOwner->GetColor();
		Node::EColor adjacentColor = adjacent->GetColor();
		
		if ((ownerColor == Node::EColor::RED || ownerColor == Node::EColor::YELLOW || ownerColor == Node::EColor::GREEN)
			&&
			(adjacentColor == Node::EColor::RED || adjacentColor == Node::EColor::YELLOW || adjacentColor == Node::EColor::GREEN))
		{
			SDL_SetRenderDrawColor(renderer, mHighlight.r, mHighlight.g, mHighlight.b, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, mBlack.r, mBlack.g, mBlack.b, 255);
		}
		
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		// Only run if antialias is off or if there is not success
		SDL_RenderDrawLineF(renderer,
			x1,
			y1,
			adjacentRelativePos.x * zoom + screenCenterPos.x,
			adjacentRelativePos.y * zoom + screenCenterPos.y);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}