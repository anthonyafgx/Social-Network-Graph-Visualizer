#include "DynamicLinesComponent.h"
#include "../GraphicsEngine.h"
#include "../Actors/Node.h"
#include "../SDL2_gfxPrimitives.h"

DynamicLinesComponent::DynamicLinesComponent(Node* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder),
	mAntialiasing(false)
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

		if (mAntialiasing)
		{
			// DO NOT ALWAYS DRAW: DO NOT USE!
			int success = aalineRGBA(renderer,
				static_cast<Sint16>(x1),
				static_cast<Sint16>(y1),
				static_cast<Sint16>(adjacentRelativePos.x * zoom + screenCenterPos.x),
				static_cast<Sint16>(adjacentRelativePos.y * zoom + screenCenterPos.y),
				50,50,50,255
			);

			if (!success)
			{
				return;
			}
			else
			{
				SDL_Log("Could not draw anti-aliased line, drawing non-antialiased one: %s", SDL_GetError());
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
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