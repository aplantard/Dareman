#pragma once

struct SDL_Texture;
class SpriteManager;

class Sprite
{
	friend class SpriteManager;

public:
	~Sprite();

	void SetSize(int aWidth, int aHeight)
	{
		mWidth = aWidth;
		mHeight = aHeight;
	}

private:
	Sprite(SDL_Texture* aTexture, int aWidth, int aHeight);

	SDL_Texture* mTexture = nullptr;
	int mWidth = 0;
	int mHeight = 0;
};