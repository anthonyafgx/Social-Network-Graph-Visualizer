#pragma once
#include "Component.h"
class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder);
};