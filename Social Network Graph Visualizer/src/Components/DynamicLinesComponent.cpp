#include "DynamicLinesComponent.h"
#include "../GraphicsEngine.h"
#include "../Actors/Node.h"

DynamicLinesComponent::DynamicLinesComponent(Node* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
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

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawLineF(renderer,
			x1,
			y1,
			adjacentRelativePos.x * zoom + screenCenterPos.x,
			adjacentRelativePos.y * zoom + screenCenterPos.y);
	}
}