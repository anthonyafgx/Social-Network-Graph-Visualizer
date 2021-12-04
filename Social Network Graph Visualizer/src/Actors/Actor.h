#pragma once
#include <vector>
#include <SDL.h>
#include "../Math.h"

class Actor
{
public:
	enum Behavior
	{
		None,
		MouseOnTop,
		MouseLeftClick,
		MouseRightClick,
		MouseDrag
	};

	enum EColor
	{
		DEFAULT,
		GREEN,
		YELLOW,
		RED
	};

public:
	Actor(class GraphicsEngine* graphics);
	virtual ~Actor();

	// Update Code
	void Update(float deltaTime);										//< method called from Graphics Engine class.	(not overridable)
	void UpdateComponents(float deltaTime);								//< called from Update() method.	(not overridable)
	virtual void UpdateActor(float deltaTime);							//< Child Actor's specific code.	(overridable)

	void ProcessInput(const Uint8* state);								//< Process Actor's input (not overridable)
	virtual void ActorInput(const Uint8* state);						//< Process Specific Actor input (overridable)

	// Adders / Removers
	void AddComponent(class Component* comp);
	void RemoveComponent(Component* comp);

	// Getters / Setters
	class GraphicsEngine* GetGraphicsEngine() const { return mGraphicsEngine; }	//< Get GraphicsEngine pointer
	float GetScale() const { return mScale; }								//< Get Actor's scale
	void SetScale(float scale) { mScale = scale; }							//< Set Actor's scale

	void SetRotation(float rotation) { mRotation = rotation; }
	float GetRotation() const { return mRotation; }
	void SetPosition(const Vector2D<float>& position) { mPosition = position; }
	void SetPositionFromRelative(const Vector2D<int> relative);
	const Vector2D<float>& GetPosition() const { return mPosition; }			//< Returns a const reference Vector with position.
	const Vector2D<float>& GetFowardVector() const;								//< Returns a const reference Unitary Foward Vector.

	SDL_Rect GetRelativeRect() const { return mRelativeRect; }					//!< Relative position (top-left corner) and size relative to screen (camera).
	void SetRelativeRect(SDL_Rect rect) { mRelativeRect = rect; }

	void SetColor(EColor color) { mColor = color; }
	EColor GetColor() const { return mColor; }

	// Actors Code
	// Camera Actor Code
	virtual void Zoom(float step);
	virtual float GetZoomScale();
	virtual void SetZoomScale(float zoom) { ; }

	// Node Actor Code
	virtual const std::vector<class Node*> GetAdjacentNodes() { return { }; }
	virtual Behavior GetBehavior() { return {}; }

	// Components code
	void SetDynamicTexSize(int size) { mDynamicTexSize = size; }
	int GetDynamicTexSize() const { return mDynamicTexSize; }

private:
	class GraphicsEngine* mGraphicsEngine;	//!< Graphics Engine pointer

	// Actor's information
	Vector2D<float> mPosition;				//!< Actor's logical position
	Vector2D<float> mFowardVector;			//!< Unitary vector showing where the actor is pointing at
	float mRotation;						//!< Actor's rotation (in radians)
	float mScale;							//!< Actor's scale
	bool mOnScreen;							//!< Tell's if actor is on screen.

	SDL_Rect mRelativeRect;					//!< Relative position (top-left corner) and size relative to screen (camera).

	// Containers
	std::vector<Component*> mComponents;	//!< Actor's Components

	// Components general info
	int mDynamicTexSize;					//!< Size of the texture being used by DynamicSpriteComponent.
	EColor mColor;							//!< Color of DynamicMaskComponent.
};
