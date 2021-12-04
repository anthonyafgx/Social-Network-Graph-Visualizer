#include "Camera.h"
#include "../GraphicsEngine.h"
#include "../Components/InputComponent.h"
#include "../Components/CameraInputComponent.h"

//#define DEBUG_CAMERA

Camera::Camera(GraphicsEngine* graphics) : 
	Actor(graphics),
	mZoomScale(1.0f)

{
	CameraInputComponent* input = new CameraInputComponent(this);
	input->SetPositionCentered();
	input->SetMass(1.0f);
	input->SetAirResistance(5.0f);
	input->SetForce(1200.0f);
	input->SetMaxAcceleration(Vector2D<float>(Math::Infinity, Math::Infinity));
	input->SetUpKey(SDL_SCANCODE_UP);
	input->SetDownKey(SDL_SCANCODE_DOWN);
	input->SetLeftKey(SDL_SCANCODE_LEFT);
	input->SetRightKey(SDL_SCANCODE_RIGHT);
	input->SetZoomInKey(SDL_SCANCODE_KP_PLUS);
	input->SetZoomOutKey(SDL_SCANCODE_KP_MINUS);

#ifdef DEBUG_CAMERA
	SDL_Log("Camera Position: %f, %f", GetPosition().x, GetPosition().y);
#endif
}

Camera::~Camera()
{
	;
}