#include <SDL_render.h>

#include "Sprite.h"

Sprite::Sprite(SDL_Texture* aSdlTexture, int aWidth, int aHeight)
	: mTexture(aSdlTexture)
	, mWidth(aWidth)
	, mHeight(aHeight)
{
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(mTexture);
}