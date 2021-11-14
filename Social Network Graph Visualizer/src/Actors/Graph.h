#pragma once
#include "Actor.h"
#include <vector>
#include <unordered_map>

class Graph : public Actor
{
public:
	// Engine Methods
	Graph(class GraphicsEngine* graphics);
	~Graph();

	void UpdateActor(float deltaTime) override;

	// Graph Methods
	std::vector<const Node*> FindPath(const class Node* from, const class Node* to);

private:
	// Graph Internal Helper Methods
	bool BFS(const class Node* start, const class Node* goal, std::unordered_map<const Node*, const Node*>& parentsMap);
	std::vector<const Node*> ReconstructPath(const Node* start, const Node* goal ,std::unordered_map<const Node*, const Node*>& parentsMap);

private:
	std::vector<class Node*> mNodes;
};