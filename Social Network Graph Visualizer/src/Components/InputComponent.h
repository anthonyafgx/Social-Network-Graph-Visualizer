#pragma once
#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
public:
	InputComponent(class Actor* owner, int updateOrder = 100);
	~InputComponent();

	void ProcessInput(const Uint8* state) override;

	// Getters / Setters
	void SetForce(float force) { mForce = force; }
	void SetRotatingSpeed(float speed) { mRotatingSpeed = speed; }
	
	void SetUpKey(int key) { mUpKey = key; }
	void SetDownKey(int key) { mDownKey = key; }
	void SetLeftKey(int key) { mLeftKey = key; }
	void SetRightKey(int key) { mRightKey = key; }

private:
	int mUpKey;
	int mDownKey;
	int mLeftKey;
	int mRightKey;

	float mForce;
	float mRotatingSpeed;
};