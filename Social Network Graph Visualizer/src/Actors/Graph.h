#pragma once
#include "Actor.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "Node.h"

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
	bool VerifyNode(int id);
	
	void AddRelation(int idA, int idB);
	void RemoveRelation(int idA, int idB);
	bool HaveRelation(int idA, int idB, bool print = true);
	bool HaveRelation(class Node* a, class Node* b, bool print = true);
	bool HighlightRelation(int idA, int idB); //< Graphically show if relation exists

	// Getters / Setters
	const std::unordered_map<int, class Node*>& GetNodes() { return mNodes; }


	std::vector<const class Node*> FindPath(const class Node* from, const class Node* to);

private:
	// Graph Internal Helper Methods
	bool BFS(const class Node* start, const class Node* goal, std::unordered_map<const Node*, const Node*>& parentsMap);
	std::vector<const Node*> ReconstructPath(const Node* start, const Node* goal ,std::unordered_map<const Node*, const Node*>& parentsMap);

	// Engine Helper Methods
	void RepositionNode(class Node* a, class Node* b);
	bool IsColliding(Node* a);				// true if [a] is colliding with any node in the graph
	bool AreColliding(Node* a, Node* b);	// true if [a] and [b] are colliding.

	// Highlight manager (time sets global highlighting timer)
	void HighlightNode(int id, Node::EColor color, float time = 3.0f);
	void StopAllHighlighting();

private:
	std::unordered_map<int, class Node*> mNodes;

private:
	float mHighlightTimeout;				//< Time that nodes will be highlighted during HighlightRelation()
	std::vector<Node*> mCurrentlyHighliting;//< Nodes that are being highlighted by graph
};