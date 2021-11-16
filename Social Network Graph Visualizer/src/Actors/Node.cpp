#include "Node.h"
#include "Graph.h"
#include "../GraphicsEngine.h"
#include "../Components/SpriteComponent.h"
#include "../Components/InputComponent.h"
#include "../Components/DynamicSpriteComponent.h"
#include "../Components/DynamicLinesComponent.h"
#include "../Components/DynamicTextComponent.h"
#include <string>

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Node::Node(GraphicsEngine* graphics, Graph* graph, int id, std::string name) : 
	Actor(graphics),
	mGraph(graph),
	mId(id),
	mName(name),
	mDiameter(64)	//< Diameter of the node, used to collisions and graphics (in pixels)
{  
	DynamicSpriteComponent* sprite = new DynamicSpriteComponent(this);
	sprite->AddTexture("Assets/x32/blue.png");
	sprite->AddTexture("Assets/x64/blue.png");
	sprite->AddTexture("Assets/x128/blue.png");
	sprite->AddTexture("Assets/x256/blue.png");
	sprite->AddTexture("Assets/x512/blue.png");
	sprite->AddTexture("Assets/x1024/blue.png");
	sprite->SetDefaultSize(mDiameter);
	
	new DynamicLinesComponent(this);				//< Draws relation lines.

	DynamicTextComponent* text = new DynamicTextComponent(this);
	text->SetText(std::to_string(mId));

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