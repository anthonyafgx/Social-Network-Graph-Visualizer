#pragma once
#include "Actor.h"

class Node : public Actor
{
public:
	Node(class GraphicsEngine* graphics);
	~Node();

	void UpdateActor(float deltaTime) override;
};