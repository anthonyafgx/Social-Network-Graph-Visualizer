#pragma once
#include "InputComponent.h"

class CameraInputComponent : public InputComponent
{
public:
	CameraInputComponent(class Actor* owner, int updateOrder = 100);
	~CameraInputComponent();

	void ProcessInput(const Uint8* state) override;
	void SetZoomInKey(int key) { mZoomInKey = key; }
	void SetZoomOutKey(int key) { mZoomOutKey = key; }
	void SetZoomStep(float step) { mZoomStep = step; }

private:
	int mZoomInKey;
	int mZoomOutKey;
	float mZoomStep;	//< By how much it zooms every time button is pressed
};