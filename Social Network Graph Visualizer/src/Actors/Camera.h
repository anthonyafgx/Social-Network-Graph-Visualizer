#pragma once
#include "Actor.h"
class Camera : public Actor
{
public:
	Camera(class GraphicsEngine* graphics);
	~Camera();

	void Zoom(float step) override { mZoomScale += step; }
 	float GetZoomScale() override { return mZoomScale; }
	void SetZoomScale(float zoom) override { mZoomScale = zoom; }

private:
	float mZoomScale;
};

