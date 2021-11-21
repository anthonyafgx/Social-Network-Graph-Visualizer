#pragma once
#include "Actor.h"
#include <unordered_map>
#include <string>

class Mouse : public Actor
{
public:
	enum State
	{
		None,
		Pressed,
		Drag
	};

public:
	Mouse(class GraphicsEngine* graphics);

	void ActorInput(const Uint8* state) override;
	void UpdateActor(float deltaTime) override;

	// Getters / Setters
	State GetLeftState() const { return mLeftCurrState; }
	State GetRightState() const { return mRightCurrState; }

private:
	// Adjustable Variables
	const float mTimeToDrag;		//< Time it takes to consider a series of "pressed states" a drag.
	float mLeftPressTimeout;		//< Tracks the time the left button is pressed.
	float mRightPressTimeout;		//< Tracks the time the right button is pressed.

	// Mouse Buttons Info
	State mLeftPrevState;
	State mRightPrevState;
	State mLeftCurrState;
	State mRightCurrState;
};