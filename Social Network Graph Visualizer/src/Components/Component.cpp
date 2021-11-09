#include "Component.h"
#include "../Actors/Actor.h"

Component::Component(Actor* actor, int updateOrder) : mOwner(actor)
{
	// Add itself to the Actor's mComponents vector
	mOwner->AddComponent(this);
}

Component::~Component()
{
	// Removes itself from the Actor's mComponents vector
	mOwner->RemoveComponent(this);
}

/*
 * Updates the component based in deltatime
 */
void Component::Update(float deltaTime)
{
	;
}

void Component::ProcessInput(const Uint8* state)
{
	;
}