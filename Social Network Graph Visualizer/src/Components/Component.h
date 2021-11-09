#pragma once
#include <SDL.h>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	// Update
	virtual void Update(float deltaTime);			//< Method called from Actor (overridable).
	virtual void ProcessInput(const Uint8* state);	//< Method called from Actor (overridable).

	// Getters / Setters
	int GetUpdateOrder() const { return mUpdateOrder; }
	Actor* GetOwner() const { return mOwner; }

protected:
	Actor* mOwner;								//!< Actor's to which the Component belongs to

private:
	int mUpdateOrder;							//!< Component's update order
};