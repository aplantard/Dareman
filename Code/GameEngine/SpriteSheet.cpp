#include <SDL_render.h>
#include <SDL_image.h>
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(const char* aPath, int aRow, int aCol)
{
	mImage = IMG_Load(aPath);

	mClip.w = mImage->w / aCol;
	mClip.h = mImage->h / aRow;
}

SpriteSheet::~SpriteSheet()
{
	SDL_FreeSurface(mImage);
}

void SpriteSheet::SelectSprite(int aCol, int aRow)
{ 
	mClip.x = aCol * mClip.w;
	mClip.y = aRow * mClip.h;

}
