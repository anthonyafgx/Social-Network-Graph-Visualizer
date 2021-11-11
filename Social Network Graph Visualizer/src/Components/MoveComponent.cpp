#include "MoveComponent.h"
#include "../Actors/Actor.h"
#include "../GraphicsEngine.h"

//#define SHOW_DATA

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) :
	Component(owner, updateOrder),
	mMass(1.0f),
	mAngularSpeed(0.0f),
	mMaxAcceleration(Vector2D<float>(5000.0f, 5000.0f)),
	mAirResistance(0.0f)
{ 
	;
}

MoveComponent::~MoveComponent()
{
	;
}

void MoveComponent::Update(float deltaTime)
{
	// Calculate and Set Rotation
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	// Calculate Acceleration
	mAcceleration.x += mSumOfForces.x / mMass;
	mAcceleration.y += mSumOfForces.y / mMass;

	// Calculate Air Resistance
	mAcceleration.x -= mAcceleration.x * mAirResistance * deltaTime;
	mAcceleration.y -= mAcceleration.y * mAirResistance * deltaTime;

	// Acceleration Limit
	if (mAcceleration.x >= mMaxAcceleration.x)
	{
		// positive limit
		mAcceleration.x = mMaxAcceleration.x;
	}
	else if (mAcceleration.x <= -mMaxAcceleration.x)
	{
		// negative limit
		mAcceleration.x = -mMaxAcceleration.x;
	}
	if (mAcceleration.y >= mMaxAcceleration.y)
	{
		// positive limit
		mAcceleration.y = mMaxAcceleration.y;
	}
	else if (mAcceleration.y <= -mMaxAcceleration.y)
	{
		// negative limit
		mAcceleration.y = -mMaxAcceleration.y;
	}

	// Calcualte Velocity
	mVelocity = mAcceleration * deltaTime;

	// Calculate and add position
	mPosition = mOwner->GetPosition();
	mPosition += mVelocity * deltaTime;
	mOwner->SetPosition(mPosition);


#ifdef SHOW_DATA

	// Get total acceleration
	float accelerationMod = sqrtf(powf(mAcceleration.x, 2) + powf(mAcceleration.y, 2));
	float velocityMod = sqrtf(powf(mVelocity.x, 2) + powf(mVelocity.y, 2));

	// Show
	SDL_Log("[%i] Acceleration: %f", this, accelerationMod);
	SDL_Log("[%i]\t Velocity: %f", this, velocityMod);

#endif

	// Reset Sum Of Forces and Angular Speed
	mSumOfForces = Vector2D<float>(0.0f, 0.0f);
	mAngularSpeed = 0.0f;

	// Parent Method Call
	Component::Update(deltaTime);
}

void MoveComponent::AddForce(const Vector2D<float>& force)
{
	mSumOfForces += force;
}

void MoveComponent::SetPosition(Vector2D<float> pos)
{
	mOwner->SetPosition(pos);
}

void MoveComponent::SetPositionCentered()
{
	SetPosition(mOwner->GetGraphicsEngine()->GetScreenCenterF());
}