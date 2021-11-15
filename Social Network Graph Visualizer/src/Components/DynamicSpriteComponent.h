#pragma once
#include "SpriteComponent.h"
#include <unordered_map>
#include <string>

/*
* SpriteComponent that draws relative to camera 
* and allows different texture sizes depending on camera zoom.
* path example: ../x64/circle.png
* path exmaple: ../x256/circle.png
* NOTE: ONLY ADD 2^n SIZES TEXTURES (x64, x128, x256, x512, x1024)
* x64 means 64 pixels WIDE.
*/
class DynamicSpriteComponent : public SpriteComponent
{
public:
	DynamicSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Draw(SDL_Renderer* renderer) override;

	void SetDefaultSize(int size) { mDefaultSize = size; }
	void AddTexture(std::string path);

private:
	float mNormalizeFactor;
	int mDefaultSize;		// Default Width Size.
	int mCurrentSize;		// Current Texture Size.

	/*
	* Textures of different sizes map.
	* Key = int with size of 2^n (Example: 64, 128, 256, etc.)
	* Value = path to texture
	*/
	std::unordered_map<int, std::string> mTexturesMap;
};