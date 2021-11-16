#include "CameraInputComponent.h"
#include "../Actors/Actor.h"
#include <iostream>

//#define DEBUG_CAMERA_INPUT

CameraInputComponent::CameraInputComponent(Actor* owner, int updateOrder) :
	InputComponent(owner, updateOrder),
	mZoomStep(0.025f),
	mMaxZoomIn(10.0f),
	mMaxZoomOut(0.15f)
{

}

CameraInputComponent::~CameraInputComponent()
{
	;
}

void CameraInputComponent::ProcessInput(const Uint8* state)
{
	float zoom = mOwner->GetZoomScale();

	// Process Input
	if (state[mZoomInKey])
	{
		zoom += mZoomStep;
	}
	if (state[mZoomOutKey])
	{
		zoom -= mZoomStep;
	}

	// Mantain zoom within margins
	if (zoom <= mMaxZoomOut)
	{
		zoom = mMaxZoomOut;
	}
	if (zoom >= mMaxZoomIn)
	{
		zoom = mMaxZoomIn;
	}

	// Set Zoom
	mOwner->SetZoomScale(zoom);
	

#ifdef DEBUG_CAMERA_INPUT
	std::cout << "Zoom Scale: " << mOwner->GetZoomScale() << "\n";
#endif

	// Run Parent Method
	InputComponent::ProcessInput(state);
}