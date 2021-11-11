#include "CameraInputComponent.h"
#include "../Actors/Actor.h"

CameraInputComponent::CameraInputComponent(Actor* owner, int updateOrder) :
	InputComponent(owner, updateOrder),
	mZoomStep(0.025f)
{

}

CameraInputComponent::~CameraInputComponent()
{
	;
}

void CameraInputComponent::ProcessInput(const Uint8* state)
{
	if (state[mZoomInKey])
	{
		mOwner->Zoom(mZoomStep);
	}
	if (state[mZoomOutKey])
	{
		mOwner->Zoom(-mZoomStep);
	}

	// Run Parent Method
	InputComponent::ProcessInput(state);
}