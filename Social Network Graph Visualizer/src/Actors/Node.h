#pragma once
#include "Actor.h"
#include <vector>
#include <string>

class Node : public Actor
{
public:
	Node(class GraphicsEngine* graphics, class Graph* graph);
	~Node();

	// Graphics Engine Methods
	void UpdateActor(float deltaTime) override;

	// Graph Methods
	int GetId() const { return mId; }
	void SetId(int id) { mId = id; }
	std::string GetName() const { return mName; }
	void SetName(std::string name) { mName = name; }

public:
	std::vector<Node*> mAdjacent;	//< Adjacent Nodes (adjacency list)

private:
	class Graph* mGraph;
	int mId;
	std::string mName;
};