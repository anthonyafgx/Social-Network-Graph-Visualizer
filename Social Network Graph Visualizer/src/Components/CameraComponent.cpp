#include "CameraComponent.h"
#include "Component.h"
#include "../Actors/Actor.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder) : Component(owner, updateOrder) { ; }

void CameraComponent::Update(float deltaTime)
{
	;
}