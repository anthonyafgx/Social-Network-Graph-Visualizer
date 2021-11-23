#include "Graph.h"
#include "../GraphicsEngine.h"
#include "Actor.h"
#include "Node.h"
#include <queue>
#include <iostream>

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Graph::Graph(GraphicsEngine* graphics) :
	Actor(graphics)
{
	;
}

Graph::~Graph()
{
	;
}

void Graph::UpdateActor(float deltaTime)
{
	;
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

	// Destroy and Remove Node.
	delete mNodes[id];
	mNodes.erase(id);
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


std::vector<const Node*> Graph::FindPath(const Node* from, const Node* to)
{
	// TO DO: Check if nodes exist

	std::unordered_map<const Node*, const Node*> parentsMap;
	std::vector<const Node*> path;

	if (BFS(to, from, parentsMap))
	{
		path = ReconstructPath(from, to, parentsMap);
		return path;
	}
	else
	{
		std::cout << "INFO: No path was found from Node address: " << from << " to Node address: " << to << std::endl;
		return std::vector<const Node*> {};
	}
}

/*
* Makes an Breadth First Search from "start" node, to "goal" node.
* @param start		Indicates the node from which BFS will start.
* @param goal		Indicates the node which wants to be found.
* @param parentsMap	Is a map where key is the node and the value is it's parent.
* @return			Value indicating whether or not path from start to goal was found.
*/
bool Graph::BFS(const Node* start, const Node* goal, std::unordered_map<const Node*, const Node*>& parentsMap)
{
	std::queue<const Node*> queue;	//< queue of Nodes that will be searched.
	bool foundPath = false;			//< indicates if a path from start to goal was found.

	queue.emplace(start);

	while (!queue.empty())
	{
		// STEP 1: Analize node in queue and delete from queue
		const Node* currentNode = queue.front();

		// dequeue current Node
		queue.pop();

		if (currentNode == goal)
		{
			foundPath = true;
			break;
		}

		// STEP 2: Enqueue current node's adjacent nodes and add to parentsMap
		for (const Node* next : currentNode->mAdjacent)
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
*/
std::vector<const Node*> Graph::ReconstructPath(const Node* start, const Node* goal, std::unordered_map<const Node*, const Node*>& parentsMap)
{
	std::vector<const Node*> path;

	// Navigate the path down to start, from goal.
	for (auto node = goal; node != nullptr; node = parentsMap[node])
	{
		path.emplace_back(node);
	}

	if (path.back() == goal)
	{
		return path;
	}
	else
	{
		std::cout << "INFO: No path was found from Node address: " << start << " to Node address: " << goal << std::endl;
		return std::vector<const Node*> {};
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
		float randomDistance = Math::Random(3, 5);			//< (modifiable) distance from one node to another (n times diameter).

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