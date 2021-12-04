#include "Node.h"
#include "Graph.h"
#include "Mouse.h"
#include "../GraphicsEngine.h"
#include "../Components/InputComponent.h"
#include "../Components/DynamicSpriteComponent.h"
#include "../Components/DynamicOpacityComponent.h"
#include "../Components/DynamicLinesComponent.h"
#include "../Components/DynamicTextComponent.h"
#include "../Components/DynamicMaskComponent.h"
#include <string>
#include <iostream>

//#define NODE_DEBUG

// TO DO: Add an "Add Node" to Graph.cpp, which Node.cpp can call and add himself to Graph.

Node::Node(GraphicsEngine* graphics, Graph* graph, int id, std::string name) : 
	Actor(graphics),
	mGraph(graph),
	mId(id),
	mName(name),
	mDiameter(64),	//< Diameter of the node, used to collisions and graphics (in pixels)
	mBehavior(Node::None),
	mColorOverride(false),
	mHighlightingNodes(false)
{  
	DynamicSpriteComponent* sprite = new DynamicSpriteComponent(this);
	sprite->AddTexture("Assets/x32/circle.png");
	sprite->AddTexture("Assets/x64/circle.png");
	sprite->AddTexture("Assets/x128/circle.png");
	sprite->AddTexture("Assets/x256/circle.png");
	sprite->AddTexture("Assets/x512/circle.png");
	sprite->SetDefaultSize(mDiameter);

	DynamicOpacityComponent* opacity = new DynamicOpacityComponent(this);
	opacity->AddTexture("Assets/x32/white_opacity.png");
	opacity->AddTexture("Assets/x64/white_opacity.png");
	opacity->AddTexture("Assets/x128/white_opacity.png");
	opacity->AddTexture("Assets/x256/white_opacity.png");
	opacity->AddTexture("Assets/x512/white_opacity.png");

	new DynamicMaskComponent(this);
	
	new DynamicLinesComponent(this);				//< Draws relation lines.

	DynamicTextComponent* text = new DynamicTextComponent(this);
	text->SetText(std::to_string(mId));

	MoveComponent* move = new MoveComponent(this);
	move->SetPositionCentered();
}

Node::~Node()
{

}

// TO DO: Write Behavior and Actor's Input to Actor.h
void Node::ActorInput(const Uint8* state)
{
	// STEP 1: Update mBehavior
	Mouse* mouse = GetGraphicsEngine()->GetMouse();
	SDL_Rect mouseRect = mouse->GetRelativeRect();

	// Detect collision between Node and mouse
	bool mouseCollision = false;

	SDL_Rect nodeRect = GetRelativeRect();	// x and y top left corner
	nodeRect.x += nodeRect.w / 2;			// get x center
	nodeRect.y += nodeRect.h / 2;			// get y center
	int nodeRadius = nodeRect.w / 2;		//< Node Relative Radius (relative to camera's zoom)

	Vector2D<int> vec = Vector2D<int>(mouseRect.x, mouseRect.y) - Vector2D<int>(nodeRect.x, nodeRect.y);

	mouseCollision = vec.LengthSq() <= nodeRadius * nodeRadius;

	if (mColorOverride == false)
	{
		if (mouseCollision)
		{
			// Detect Left Click or Drags
			Mouse::State leftButton = mouse->GetLeftState();	//< Mouse's left button state
			Mouse::State rightButton = mouse->GetRightState();	//< Mouse's right button state

			if (leftButton == Mouse::None)	// if left button not clicked
			{
				mBehavior = Actor::MouseOnTop;
			}
			else if (leftButton == Mouse::Pressed)
			{
				mBehavior = Actor::MouseLeftClick;
				SetColor(EColor::RED);

				// HIGHLIGHT Adjacent Nodes (Recolor YELLOW)
				for (auto& node : mAdjacent)
				{
					mHighlightingNodes = true;
					node->SetColorOverride(true);
					node->SetColor(EColor::YELLOW);
				}
			}
			else if (leftButton == Mouse::Drag)
			{
				mBehavior = Actor::MouseDrag;
			}
		}
		else
		{
			mBehavior = Actor::None;
			SetColor(EColor::DEFAULT);
			
			if (mHighlightingNodes)
			{
				// UNHIGHLIGHT Adjacent Nodes
				for (auto& node : mAdjacent)
				{
					node->SetColorOverride(false);
				}
				mHighlightingNodes = false;
			}
		}
	}
}

void Node::UpdateActor(float deltaTime)
{
	SDL_Rect mousePos = GetGraphicsEngine()->GetMouse()->GetRelativeRect();
	Vector2D<int> pos{ static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };
	// Detect mouse drags
	if (mBehavior == Actor::MouseDrag)
	{
		
		//SetPositionFromRelative(pos);
		
		
	}
	if (mId == 5)
	{
		//std::cout << "Node Relative Pos: " << GetRelativeRect().x + GetRelativeRect().w / 2 << "," << GetRelativeRect().y + GetRelativeRect().h / 2 << "\n";
		//std::cout << "Mouse Relative Pos: " << mousePos.x << ", " << mousePos.y << "\t";
		//std::cout << "Camera Zoom: " << GetGraphicsEngine()->GetCameraZoom() << "\t";
	}


#ifdef NODE_DEBUG
	if (mBehavior == Node::MouseOnTop)
	{
		SDL_Log("(Node) %i MouseOnTop", mId);
	}
	if (mBehavior == Node::MouseLeftClick)
	{
		SDL_Log("(Node) %i MouseLeftClick", mId);
	}
	if (mBehavior == Node::MouseRightClick)
	{
		SDL_Log("(Node) %i MouseRightClick", mId);
	}
	if (mBehavior == Node::MouseDrag)
	{
		SDL_Log("(Node) %i Drag", mId);
	}
#endif
}