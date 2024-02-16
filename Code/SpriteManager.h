#pragma once

#include <map>
#include <string>
class Sprite;

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();
	SpriteManager(const SpriteManager& anOther) = delete;

	Sprite* CreateRGBSprite(int aWidth, int aHeight, int aRGBAColor);
	Sprite* LoadSprite(const char* aPath);
	Sprite* GetSprite(const char* aPath);

private:

	std::map<std::string, Sprite*> mSprites;

};