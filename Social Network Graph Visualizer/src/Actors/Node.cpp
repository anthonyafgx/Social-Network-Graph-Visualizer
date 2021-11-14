#include "Node.h"
#include "../GraphicsEngine.h"
#include "../Components/SpriteComponent.h"
#include "../Components/InputComponent.h"

Node::Node(GraphicsEngine* graphics) : Actor(graphics) 
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