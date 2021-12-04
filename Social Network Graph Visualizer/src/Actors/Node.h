#pragma once
#include "Actor.h"
#include <vector>
#include <string>

class Node : public Actor
{
public:
	
	// TO DO: Write Behavior and Actor's Input to Actor.h
	// Moved Behavior to Actor.h 
	/*enum Behavior
	{
		None,
		MouseOnTop,
		MouseLeftClick,
		MouseRightClick,
		MouseDrag
	};*/

public:
	Node(class GraphicsEngine* graphics, class Graph* graph, int id, std::string name);
	~Node();

	// Graphics Engine Methods
	void UpdateActor(float deltaTime) override;
	void ActorInput(const Uint8* state) override;

	// Graph Methods
	int GetId() const { return mId; }
	std::string GetName() const { return mName; }

	// Getters / Setters
	int GetDiameter() const { return mDiameter; }
	const std::vector<class Node*> GetAdjacentNodes() override { return mAdjacent; }
	Behavior GetBehavior() override { return mBehavior; }
	void SetColorOverride(bool value) { mColorOverride = value; }

public:
	// Graph Variables
	std::vector<Node*> mAdjacent;	//< Adjacent Nodes (adjacency list)

private:
	// Graph Variables
	class Graph* mGraph;
	const int mId;
	const std::string mName;

	// Engine Variables
	int mDiameter;					//< Node's logical diameter (non relative to camera).
	Behavior mBehavior;				//< Node's behavior depending on mouse.

	// Color Override
	bool mColorOverride;			//< Skips the recoloring part of ProcessInput().
	bool mHighlightingNodes;		//< Tells if node is currently highliting nodes.
};