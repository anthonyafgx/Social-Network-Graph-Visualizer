#include "Node.h"
#include "Graph.h"
#include "../GraphicsEngine.h"
#include "../Components/SpriteComponent.h"
#include "../Components/InputComponent.h"
#include "../Components/DynamicSpriteComponent.h"

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Node::Node(GraphicsEngine* graphics, Graph* graph) : 
	Actor(graphics),
	mGraph(graph)
{  
	DynamicSpriteComponent* sprite = new DynamicSpriteComponent(this);
	sprite->AddTexture("Assets/x32/blue.png");
	sprite->AddTexture("Assets/x64/blue.png");
	sprite->AddTexture("Assets/x128/blue.png");
	sprite->AddTexture("Assets/x256/blue.png");
	sprite->AddTexture("Assets/x512/blue.png");
	sprite->AddTexture("Assets/x1024/blue.png");

	//SpriteComponent* sprite = new SpriteComponent(this);
	//sprite->SetTexture("Assets/x64/blue.png");

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