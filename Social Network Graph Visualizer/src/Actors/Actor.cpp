#include "Actor.h"
#include "../Components/Component.h"
#include "../GraphicsEngine.h"

Actor::Actor(GraphicsEngine* graphics) : mGraphicsEngine(graphics), mScale(1.0f), mRotation(0.0f)
{
	// Add itself to the mActors vector
	mGraphicsEngine->AddActor(this);

#ifdef DEBUG_ACTOR
	printf("DEBUG: Actor Created!\n");
#endif // DEBUG_ACTOR
}

/*
 * Destructor
 */
Actor::~Actor()
{
	// Remove components
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	// Remove itself from mActors vector
	mGraphicsEngine->RemoveActor(this);

#ifdef DEBUG_ACTOR
	printf("DEBUG: Actor Destroyed!\n");
#endif // DEBUG_ACTOR
}


// UPDATES

/*
 * Update Components and Update Actor's specific code.
 */
void Actor::Update(float deltaTime)
{
	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto& comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{

}

void Actor::ProcessInput(const Uint8* state)
{
	for (auto& comp : mComponents)
	{
		comp->ProcessInput(state);
	}
}

// ADDERS / REMOVERS

/*
 * Adds a component taking into account update order
 */
void Actor::AddComponent(Component* comp)
{
	int myUpdateOrder = comp->GetUpdateOrder();

	auto itr = mComponents.begin();

	for (; itr != mComponents.end(); itr++)
	{
		if ((*itr)->GetUpdateOrder() >= myUpdateOrder)
		{
			break;
		}
	}

	mComponents.insert(itr, comp);
}

/*
 * Removes a component
 */
void Actor::RemoveComponent(Component* comp)
{
	// NOTE: CHECK FOR MEMORY LEAKS WITH THE erase METHOD.
	auto itr = std::find(mComponents.begin(), mComponents.end(), comp);

	if (itr != mComponents.end())
	{
		mComponents.erase(itr);
	}
}

const Vector2D<float>& Actor::GetFowardVector() const
{
	return Vector2D<float>{ cosf(mRotation), -sinf(mRotation) };
}

void Actor::Zoom(float step)
{

}

float Actor::GetZoomScale() { return 0.0f; }