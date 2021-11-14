#pragma once
#include "Actor.h"
#include <vector>
#include <unordered_map>
#include <string>

class Graph : public Actor
{
public:
	// Engine Methods
	Graph(class GraphicsEngine* graphics);
	~Graph();

	void UpdateActor(float deltaTime) override;

	// Graph Methods
	void InsertNode(int id, std::string name);
	void RemoveNode(int id);
	
	void AddRelation(int idA, int idB);
	void RemoveRelation(int idA, int idB);
	bool HaveRelation(int idA, int idB, bool print = true);
	bool HaveRelation(class Node* a, class Node* b, bool print = true);


	std::vector<const class Node*> FindPath(const class Node* from, const class Node* to);

private:
	// Graph Internal Helper Methods
	bool BFS(const class Node* start, const class Node* goal, std::unordered_map<const Node*, const Node*>& parentsMap);
	std::vector<const Node*> ReconstructPath(const Node* start, const Node* goal ,std::unordered_map<const Node*, const Node*>& parentsMap);

private:
	std::unordered_map<int, class Node*> mNodes;
};