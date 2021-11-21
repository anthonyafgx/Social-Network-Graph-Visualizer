#include "Mouse.h"
#include "../GraphicsEngine.h"
#include <iostream>

//#define MOUSE_DEBUG_BUTTONS
//#define MOUSE_DEBUG_POSITION

Mouse::Mouse(GraphicsEngine* graphics) :
	Actor(graphics),
	mLeftPrevState(Mouse::None),
	mRightPrevState(Mouse::None),
	mTimeToDrag(0.15f),
	mLeftPressTimeout(mTimeToDrag),
	mRightPressTimeout(mTimeToDrag)
{

}

void Mouse::ActorInput(const Uint8* state)
{
	// Update Previous Variables
	mLeftPrevState = mLeftCurrState;
	mRightPrevState = mRightCurrState;

	// Get Info
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);

	// Update Mouse Rect
	SetRelativeRect(SDL_Rect{ x, y, 1, 1 });

	// Update Mouse Buttons' State
	if ((buttons & SDL_BUTTON_LMASK) != 0)
	{
		if (mLeftCurrState != Mouse::Drag)
		{
			mLeftCurrState = Mouse::Pressed;
		}
	}
	else
	{
		mLeftCurrState = Mouse::None;
	}

	if ((buttons & SDL_BUTTON_RMASK) != 0)
	{
		if (mRightCurrState != Mouse::Drag)
		{
			mRightCurrState = Mouse::Pressed;
		}
	}
	else
	{
		mRightCurrState = Mouse::None;
	}
}

void Mouse::UpdateActor(float deltaTime)
{
	// STEP 1: DETECT MOUSE DRAGS.
	
	// Restart timeout
	if (mLeftPrevState == Mouse::None && mLeftCurrState == Mouse::Pressed)
	{
		mLeftPressTimeout = mTimeToDrag;
	}
	if (mRightPrevState == Mouse::None && mRightCurrState == Mouse::Pressed)
	{
		mRightPressTimeout = mTimeToDrag;
	}

	// Update timeout
	if (mLeftPrevState == Mouse::Pressed && mLeftCurrState == Mouse::Pressed)
	{
		mLeftPressTimeout -= deltaTime;
	}
	if (mRightPrevState == Mouse::Pressed && mRightCurrState == Mouse::Pressed)
	{
		mRightPressTimeout -= deltaTime;
	}

	// Update state.
	if (mLeftPressTimeout <= 0 && mLeftCurrState != Mouse::None)
	{
		mLeftCurrState = Mouse::Drag;
		mLeftPressTimeout = 0;
	}
	if (mRightPressTimeout <= 0 && mRightCurrState != Mouse::None)
	{
		mRightCurrState = Mouse::Drag;
		mRightPressTimeout = 0;
	}
#ifdef MOUSE_DEBUG_BUTTONS

	if (mLeftCurrState == Mouse::Pressed && mLeftPrevState == Mouse::None)
	{
		std::cout << "INFO (Mouse): Left Mouse Button Pressed\n";
	}
	else if (mLeftCurrState == Mouse::Drag && mLeftPrevState == Mouse::Pressed)
	{
		std::cout << "INFO (Mouse): Left Mouse Button Dragging\n";
	}

	if (mRightCurrState == Mouse::Pressed && mRightPrevState == Mouse::None)
	{
		std::cout << "INFO (Mouse): Right Mouse Button Pressed\n";
	}
	else if (mRightCurrState == Mouse::Drag && mRightPrevState == Mouse::Pressed)
	{
		std::cout << "INFO (Mouse): Right Mouse Button Dragging\n";
	}

#endif

#ifdef MOUSE_DEBUG_POSITION

	std::cout << "INFO (Mouse): Mouse position relative to camera is " << GetRelativeRect().x << "," << GetRelativeRect().y << "\n";

#endif
}