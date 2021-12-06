#include "Graph.h"
#include "../GraphicsEngine.h"
#include "Actor.h"
#include "Node.h"
#include <queue>
#include <iostream>

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Graph::Graph(GraphicsEngine* graphics) :
	Actor(graphics),
	mHighlightTimeout(0.0f)
{
	;
}

Graph::~Graph()
{
	;
}

void Graph::UpdateActor(float deltaTime)
{
	/* NODE HIGHLIGHTING */
	if (mHighlightTimeout > 0.0f)
	{
		mHighlightTimeout -= deltaTime;
		mHighlightTimeout = (mHighlightTimeout < 0.0f) ? 0.0f : mHighlightTimeout;	// clamp
	}
	else // end highlighting
	{
		StopAllHighlighting();
	}

	
}

void Graph::InsertNode(int id, std::string name)
{
	// Check if id already exists in graph.
	if (mNodes[id] != nullptr)
	{
		std::cout << "INFO (InsertNode): Cannot Insert Node. Node with ID " << id << " already exists (in address " << mNodes[id] << ")\n";
		return;
	}

	// Create node
	Node* node = new Node(GetGraphicsEngine(), this, id, name);

	// Check if colliding
	int attempts = 20;
	while (IsColliding(node) && attempts != 0)
	{
		--attempts;
		Vector2D<float> newPos(		// NOTE: Random Numbers are adjustable.
			GetGraphicsEngine()->GetScreenCenterF().x + Math::Random(-250, 250),
			GetGraphicsEngine()->GetScreenCenterF().y + Math::Random(-250, 250)
		);
		node->SetPosition(newPos);
	}

	// Add node to {mNodes} map
	mNodes[id] = node;

	std::cout << "INFO (InsertNode): Node " << id << " successfully inserted.\n";
}

void Graph::RemoveNode(int id)
{
	// Check if node with ID exists.
	if (mNodes[id] == nullptr)
	{
		std::cout << "INFO (RemoveNode): Cannot Remove Node. Node with ID " << id << " does not exist.\n";
		return;
	}

	// Remove all relations.
	while (!mNodes[id]->mAdjacent.empty())
	{
		RemoveRelation(id, mNodes[id]->mAdjacent.back()->GetId());
	}

	// Destroy and Remove Node.
	delete mNodes[id];
	mNodes.erase(id);
}

bool Graph::VerifyNode(int id)
{
	StopAllHighlighting();
	if (!mNodes[id])
	{
		std::cout << "INFO (VerifyNode): Node with ID: " << id << " does not exits.\n";
		return false;
	}
	else
	{
		std::cout << "INFO (VerifyNode): Node with ID: " << id << " do exist. Currently highlighted.\n";
		HighlightNode(id, Node::EColor::GREEN);
		return true;
	}
}

void Graph::RemoveAllNodes()
{
	while (!mNodes.empty())
	{
		RemoveNode(mNodes.begin()->second->GetId());
	}
}

void Graph::AddRelation(int idA, int idB)
{
	// Ids should be different
	if (idA == idB)
	{
		std::cout << "INFO (AddRelation): Cannot Relate ID " << idA << " with itself!\n";
		return;
	}

	// Both nodes should exist
	if (mNodes[idA] == nullptr || mNodes[idB] == nullptr)
	{
		std::cout << "INFO (AddRelation): Cannot Relate ID " << idA << " with ID " << idB << " because one or both nodes do not exist.\n";
		return;
	}

	// Relation should not previously exist
	if (HaveRelation(idA, idB, false))
	{
		std::cout << "INFO (AddRelation): Relation between Node " << idA << " and Node " << idB << " already exists.\n";
		return;
	}

	// SUCESS Create Relation.
	
	// Reposition Nodes before creating relation
	RepositionNode(mNodes[idA], mNodes[idB]);

	// Create Relation
	mNodes[idA]->mAdjacent.emplace_back(mNodes[idB]);	// add Node B to Node A's Adjacent Nodes.
	mNodes[idB]->mAdjacent.emplace_back(mNodes[idA]);	// add Node A to Node B's Adjacent Nodes.

	std::cout << "INFO (AddRelation): Relation between Node " << idA << " and Node " << idB << " successfully created.\n";
}

void Graph::RemoveRelation(int idA, int idB)
{
	// Check if both nodes exist
	if (mNodes[idA] == nullptr || mNodes[idB] == nullptr)
	{
		std::cout << "INFO (RemoveRelation): Cannot Remove Relation between ID " << idA << " with ID " << idB << " because one or both nodes does not exist.\n";
		return;
	}

	Node* a = mNodes[idA];
	Node* b = mNodes[idB];

	// Check if nodes have a relation
	if (!HaveRelation(a, b, false))
	{
		std::cout << "INFO (RemoveRelation): Cannot Remove Relation because relation between ID " << idA << " and ID " << idB << " does not exist.\n";
		return;
	}

	// STEP 1: Remove Relation.
	// remove Node B from Node A's Adjacency List.
	auto itrA = std::find(a->mAdjacent.begin(), a->mAdjacent.end(), b);
	if (itrA != a->mAdjacent.end())
	{
		a->mAdjacent.erase(itrA);
	}
	else // this should never run.
	{
		std::cout << "FATAL ERROR (RemoveRelation): Could not remove Node B from Node A's adjacency list (Unknown Reason, iterator reached end of list)\n";
	}

	auto itrB = std::find(b->mAdjacent.begin(), b->mAdjacent.end(), a);
	if (itrB != b->mAdjacent.end())
	{
		b->mAdjacent.erase(itrB);
	}
	else // this should never run.
	{
		std::cout << "FATAL ERROR (RemoveRelation): Could not remove Node A from Node B's adjacency list (Unknown Reason, iterator reached end of list)\n";
	}
}

bool Graph::HaveRelation(int idA, int idB, bool print)
{
	if (HaveRelation(mNodes[idA], mNodes[idB]))	// have relation
	{
		if (print)
		{
			std::cout << "INFO (HaveRelation): Node " << idA << " and Node " << idB << " have a Relation.\n";
		}
		return true;
	}
	else   // do not have relation
	{
		if (print)
		{
			std::cout << "INFO (HaveRelation): Node " << idA << " and Node " << idB << " do not have a Relation";
			if (mNodes[idA] == nullptr || mNodes[idB] == nullptr)
			{
				std::cout << " because one or both Nodes do not exist";
			}
			std::cout << ".\n";
		}
		return false;
	}
}

bool Graph::HaveRelation(Node* a, Node* b, bool print)
{
	// Check if Nodes exist
	if (!(a && b))	// if both or one of the nodes == nullptr
	{
		return false;
	}

	// Check if Nodes have relation
	auto itrA = std::find(a->mAdjacent.begin(), a->mAdjacent.end(), b);	// find Node B in Node A's Adjacency List
	auto itrB = std::find(b->mAdjacent.begin(), b->mAdjacent.end(), a);	// find Node A in Node B's Adjacency List

	if (itrA != a->mAdjacent.end() && itrB != b->mAdjacent.end())	// if Nodes exist mutually inside Node A and Node B adjacency lists.
	{
		return true;
	}
	else // if not found mutually inside adjacency lists.
	{
		return false;
	}
}

bool Graph::HighlightRelation(int idA, int idB)
{
	StopAllHighlighting();
	if (HaveRelation(idA, idB, true))
	{	
		Node* nodeA = mNodes[idA];
		Node* nodeB = mNodes[idB];
		
		// Highlight nodes for x secs
		HighlightNode(idA, Node::EColor::GREEN);
		HighlightNode(idB, Node::EColor::GREEN);
		return true;
	}
	else
	{
		// Indicate nodes do not have relation.
		;
	}
}

bool Graph::HighlightPath(int idFrom, int idTo)
{
	Node* from = mNodes[idFrom];
	Node* to = mNodes[idTo];

	// Check if nodes exist
	if (!(from && to))	// if one or both nodes == nullptr
	{
		std::cout << "INFO (Highlight Path): Cannot Find Path because one or both nodes do not exist.\n";
		return false;
	}

	// Find Path
	std::vector<Node*> path = FindPath(from, to);

	if (path.size() > 0) // if path found
	{
		// Highligh middle nodes yellow
		for (Node* node : path)
		{
			if (node == path.front())
			{
				// Highlight start node green
				HighlightNode(path.front(), Node::EColor::GREEN);
				continue;
			}
			if (node == path.back())
			{
				// Highlight end node red
				HighlightNode(path.back(), Node::EColor::RED);
				continue;
			}

			// Highlight middle nodes yellow
			HighlightNode(node, Node::EColor::YELLOW);
		}
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<Node*> Graph::FindPath(Node* from, Node* to)
{
	// Check if nodes exist
	if (!(from && to))	// if one or both nodes == nullptr
	{
		std::cout << "INFO (FindPath): Cannot Find Path because one or both nodes do not exist.\n";
		return std::vector<Node*> {};
	}


	std::unordered_map<Node*, Node*> parentsMap;
	std::vector<Node*> path;

	if (BFS(to, from, parentsMap))
	{
		path = ReconstructPath(to, from, parentsMap);
		return path;
	}
	else
	{
		std::cout << "INFO: No path was found from Node address: " << from << " to Node address: " << to << std::endl;
		return std::vector<Node*> {};
	}
}

/*
* Makes an Breadth First Search from "start" node, to "goal" node.
* @param start		Indicates the node from which BFS will start.
* @param goal		Indicates the node which wants to be found.
* @param parentsMap	Is a map where key is the node and the value is it's parent.
* @return			Value indicating whether or not path from start to goal was found.
*/
bool Graph::BFS(Node* start, Node* goal, std::unordered_map<Node*, Node*>& parentsMap)
{
	std::queue<Node*> queue;	//< queue of Nodes that will be searched.
	bool foundPath = false;			//< indicates if a path from start to goal was found.

	queue.emplace(start);

	while (!queue.empty())
	{
		// STEP 1: Analize node in queue and delete from queue
		Node* currentNode = queue.front();
		queue.pop();

		if (currentNode == goal)
		{
			foundPath = true;
			break;
		}

		// STEP 2: Enqueue current node's adjacent nodes and add to parentsMap
		for (Node* next : currentNode->mAdjacent)
		{
			// Enqueue {next} only if it has not been enqueued before.
			/* NOTE: Already visited Nodes have a non-null value stored in parentsMap
			   (does not apply to start Node) */
			if (parentsMap[next] == nullptr && next != start)	// if not visited before
			{
				// Enqueue
				queue.emplace(next);

				// Set {next}'s parent in {parentsMap}
				parentsMap[next] = currentNode;
			}
		}
	}

	return foundPath; 
}

/*
* Makes a path from goal to start.
* @param start		Indicates the node that will be found.
* @param goal		Indicates the node from which path will start.
* @param parentsMap	Is a map where key is the node and the value is it's parent.
* @return			The path from goal to start (yes, I know it is backwards).
*
* IMPORTANT NOTE:	This method returns the path in backwards order (from goal to start). If you want the path from
*					goal to start, do the BFS from goal to start, so this methods returns the path from start to goal.
*
* IMPORTANT NOTE 2: You must enter the parameters in the same order as BFS
*					Example: If you entered BFS(start, goal), you must enter ReconstructPath(start,goal) and viceversa.
*/
std::vector<Node*> Graph::ReconstructPath(Node* start, Node* goal, std::unordered_map<Node*, Node*>& parentsMap)
{
	std::vector<Node*> path;

	// Navigate the path starting from goal down to start.
	for (auto node = goal; node != nullptr; node = parentsMap[node])
	{
		path.emplace_back(node);
	}

	if (path.front() == goal && path.back() == start)
	{
		return path;
	}
	else
	{
		std::cout << "INFO: No path was found from Node address: " << start << " to Node address: " << goal << std::endl;
		return std::vector<Node*> {};
	}
}

void Graph::RepositionNode(Node* a, Node* b)
{
	// Check if both nodes exist
	if (!(a && b))	// if one or both nodes == nullptr
	{
		std::cout << "INFO (RepositionNode): Cannot reposition node because one or both nodes do not exist.\n";
		return;
	}

	int attempts = 20;	// (modifiable) max number of attempts the loop can make to fix the node

	Node* movedNode = nullptr;
	// Reposition Nodes
	do
	{
		// Count attempt
		attempts--;

		// Generate random values
		float randomAngle = Math::Random(0, Math::TwoPi);	//< random angle (in radians).
		float randomDistance = Math::Random(4, 6);			//< (modifiable) distance from one node to another (n times diameter).

		// Rule 1: If one of the nodes has no relations, move near the other node.
		if (b->mAdjacent.empty())
		{

			// Calculate unitary vector from random angle
			Vector2D<float> vec{ cosf(randomAngle), -sinf(randomAngle) };

			// Set Distance
			vec = vec * (b->GetDiameter() * randomDistance);	//< Random value determines distance (x times diameter of node)

			// Set position respect to Node A's position
			b->SetPosition(vec + a->GetPosition());

			movedNode = b;
		}
		else if (a->mAdjacent.empty())
		{
			// Calculate unitary vector from random angle
			Vector2D<float> vec{ cosf(randomAngle), -sinf(randomAngle) };

			// Set Distance
			vec = vec * (a->GetDiameter() * randomDistance);	//< Random value determines distance (x times diameter of node)

			// Set position respect to Node B's position
			a->SetPosition(vec + b->GetPosition());

			movedNode = a;
		}
	} while (IsColliding(movedNode) && attempts != 0);

}

/* @return true if node [a] is colliding with any node in the graph*/
bool Graph::IsColliding(Node* a)
{
	if (!a)	// if nullptr
	{
		return false;
	}

	for (auto itr = mNodes.begin(); itr != mNodes.end(); ++itr)
	{
		if (itr->second != a)	// check all nodes that are not Node [a]
		{
			if (AreColliding(a, itr->second))
			{
				return true;
			}
		}
	}

	return false;
}

/* @return true if nodes [a] and [b] are colliding */
bool Graph::AreColliding(Node* a, Node* b)
{
	if (!(a && b))	// if one or both nodes == nullptr
	{
		return false;
	}

	Vector2D<float> vec = b->GetPosition() - a->GetPosition();
	float totalRadius = a->GetDiameter() / 2 + b->GetDiameter() / 2;

	return vec.LengthSq() <= totalRadius * totalRadius;
}

void Graph::HighlightNode(Node* node, Node::EColor color, float time)
{
	if (!node)
	{
		std::cout << "INFO (HighlightNode): Could not highlight node because it does not exists.\n";
		return;
	}
	
	// Timer
	mHighlightTimeout = time;

	// Highlight (while overriding Node's color)
	mCurrentlyHighliting.emplace_back(node);	
	node->SetColorOverride(true);
	node->SetColor(color);
}

void Graph::HighlightNode(int id, Node::EColor color, float time)
{
	HighlightNode(mNodes[id], color, time);
}

void Graph::StopAllHighlighting()
{
	while (!mCurrentlyHighliting.empty())
	{
		mCurrentlyHighliting.back()->SetColorOverride(false);
		mCurrentlyHighliting.pop_back();
	}
}