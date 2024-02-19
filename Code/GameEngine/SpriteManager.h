#pragma once

#include <map>
#include <string>
#include "Sprite.h"

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();
	SpriteManager(const SpriteManager& anOther) = delete;

	Sprite* CreateRGBSprite(int aWidth, int aHeight, int aRGBAColor);
	Sprite* LoadSprite(const char* aPath);
	Sprite* GetSprite(const char* aPath);

	inline int GetSpriteWidth(const Sprite* aSprite) { return aSprite->mWidth; };
	inline int GetSpriteHeight(const Sprite* aSprite) { return aSprite->mHeight; };
	inline SDL_Texture* GetSpriteTexture(const Sprite* aSprite) { return aSprite->mTexture; };

private:

	std::map<std::string, Sprite*> mSprites;

};