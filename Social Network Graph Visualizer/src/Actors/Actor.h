#pragma once
#include <vector>
#include <SDL.h>
#include "../Math.h"

class Actor
{
public:
	Actor(class GraphicsEngine* graphics);
	virtual ~Actor();

	// Update Code
	void Update(float deltaTime);										//< method called from Graphics Engine class.	(not overridable)
	void UpdateComponents(float deltaTime);								//< called from Update() method.	(not overridable)
	virtual void UpdateActor(float deltaTime);							//< Child Actor's specific code.	(overridable)

	virtual void ProcessInput(const Uint8* state);						//< Process Actor's input (overridable)

	// Adders / Removers
	void AddComponent(class Component* comp);
	void RemoveComponent(Component* comp);

	// Getters / Setters
	GraphicsEngine* GetGraphicsEngine() const { return mGraphicsEngine; }	//< Get GraphicsEngine pointer
	float GetScale() const { return mScale; }								//< Get Actor's scale
	void SetScale(float scale) { mScale = scale; }							//< Set Actor's scale

	void SetRotation(float rotation) { mRotation = rotation; }
	float GetRotation() const { return mRotation; }
	void SetPosition(const Vector2D<float>& position) { mPosition = position; }
	const Vector2D<float>& GetPosition() const { return mPosition; }				//< Returns a const reference Vector with position.
	const Vector2D<float>& GetFowardVector() const;								//< Returns a const reference Unitary Foward Vector.

private:
	GraphicsEngine* mGraphicsEngine;		//!< Graphics Engine pointer

	// Actor's information
	Vector2D<float> mPosition;				//!< Actor's position on screen
	Vector2D<float> mFowardVector;			//!< Unitary vector showing where the actor is pointing at
	float mRotation;						//!< Actor's rotation (in radians)
	float mScale;							//!< Actor's scale

	// Containers
	std::vector<Component*> mComponents;	//!< Actor's Components
};
