#include "Sprite.h"

#include <SDL_render.h>

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