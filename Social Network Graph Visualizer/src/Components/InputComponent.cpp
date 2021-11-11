#include "InputComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"

InputComponent::InputComponent(class Actor* owner, int updateOrder) :
	MoveComponent(owner, updateOrder),
	mUpKey(NULL),
	mDownKey(NULL),
	mLeftKey(NULL),
	mRightKey(NULL),
	mForce(1.0f),
	mRotatingSpeed(1.0f)
{
	;
}

InputComponent::~InputComponent()
{
	;
}

void InputComponent::ProcessInput(const Uint8* state)
{
	if (state[mUpKey])
	{
		AddForce(Vector2D<float>(0.0f, -mForce));
	}
	if (state[mDownKey])
	{
		AddForce(Vector2D<float>(0.0f, mForce));
	}
	if (state[mLeftKey])
	{
		AddForce(Vector2D<float>(-mForce, 0.0f));
	}
	if (state[mRightKey])
	{
		AddForce(Vector2D<float>(mForce, 0.0f));
	}
}