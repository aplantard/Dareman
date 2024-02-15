#pragma once

struct SDL_Texture;
class Renderer;

class Sprite
{
	friend class Renderer;

public:
	~Sprite();

	void SetSize(int aWidth, int aHeight)
	{
		mWidth = aWidth;
		mHeight = aHeight;
	}

private:
	Sprite(SDL_Texture* aTexture, int aWidth, int aHeight);

	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};