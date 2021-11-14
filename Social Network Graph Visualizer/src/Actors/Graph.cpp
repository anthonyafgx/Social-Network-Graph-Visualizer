#include "Graph.h"
#include "../GraphicsEngine.h"
#include "Actor.h"
#include "Node.h"
#include <queue>
#include <iostream>

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