#include "Node.h"
#include "Graph.h"
#include "../GraphicsEngine.h"
#include "../Components/SpriteComponent.h"
#include "../Components/InputComponent.h"

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Node::Node(GraphicsEngine* graphics, Graph* graph) : 
	Actor(graphics),
	mGraph(graph)
{  
	SpriteComponent* sprite = new SpriteComponent(this);
	sprite->SetTexture("Assets/255.png");

	MoveComponent* move = new MoveComponent(this);
	move->SetPositionCentered();
}

Node::~Node()
{

}

void Node::UpdateActor(float deltaTime)
{
	;
}