#pragma once
#include "Actor.h"
#include <vector>

class Node : public Actor
{
public:
	Node(class GraphicsEngine* graphics);
	~Node();

	void UpdateActor(float deltaTime) override;

public:
	std::vector<Node*> mAdjacent;	//< Adjacent Nodes (adjacency list)
};