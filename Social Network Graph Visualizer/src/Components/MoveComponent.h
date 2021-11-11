#pragma once
#include "Component.h"
#include "../Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 100);
	virtual ~MoveComponent();

	void Update(float deltaTime) override;
	void AddForce(const Vector2D<float>& force);

	// Getters / Setters
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetAirResistance(float res) { mAirResistance = res; }
	void SetMass(float mass) { mMass = mass; }
	void SetMaxAcceleration(const Vector2D<float> acc) { mMaxAcceleration = acc; }

	void SetPosition(Vector2D<float> pos);
	void SetPositionCentered();

private:
	float mAngularSpeed;				//< Angular speed in radians / seconds

	// Physics
	Vector2D<float> mSumOfForces;		//< Sum Of Forces for the current frame
	Vector2D<float> mAcceleration;
	Vector2D<float> mVelocity;
	Vector2D<float> mPosition;
	
	float mAirResistance;				//< Force opossing to acceleration (default 0.0f)
	float mMass;
	// Vector2D<float> mMaxVelocity;
	Vector2D<float> mMaxAcceleration;	//< Max acceleration permitted (default to 50000.0f)

};