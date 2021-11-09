#pragma once
#include "Component.h"
class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder);
	virtual void Update(float deltaTime) override;
};

