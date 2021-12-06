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
	void RemoveAllNodes();
	
	void AddRelation(int idA, int idB);
	void RemoveRelation(int idA, int idB);
	bool HaveRelation(int idA, int idB, bool print = true);
	bool HaveRelation(class Node* a, class Node* b, bool print = true);
	bool HighlightRelation(int idA, int idB); //< Graphically show if relation exists

	bool HighlightPath(int idFrom, int idTo); //< Graphically Find Path

	// Getters / Setters
	const std::unordered_map<int, class Node*>& GetNodes() { return mNodes; }


	std::vector<class Node*> FindPath(class Node* from, class Node* to);

private:
	// Graph Internal Helper Methods
	bool BFS(class Node* start, class Node* goal, std::unordered_map<Node*, Node*>& parentsMap);
	std::vector<Node*> ReconstructPath(Node* start, Node* goal ,std::unordered_map<Node*, Node*>& parentsMap);

	// Engine Helper Methods
	void RepositionNode(class Node* a, class Node* b);
	bool IsColliding(Node* a);				// true if [a] is colliding with any node in the graph
	bool AreColliding(Node* a, Node* b);	// true if [a] and [b] are colliding.

	// Highlight manager (time sets global highlighting timer)
	void HighlightNode(Node* node, Node::EColor color, float time = 3.0f);
	void HighlightNode(int id, Node::EColor color, float time = 3.0f);
	void StopAllHighlighting();

private:
	std::unordered_map<int, class Node*> mNodes;

private:
	float mHighlightTimeout;				//< Time that nodes will be highlighted during HighlightRelation()
	std::vector<Node*> mCurrentlyHighliting;//< Nodes that are being highlighted by graph
};