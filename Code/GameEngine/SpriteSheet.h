#pragma once

#include <SDL.h>

class SpriteSheet
{
	friend class Renderer;

public:
	SpriteSheet(const char* aPath, int aRow, int aCol);
	~SpriteSheet();

	void SelectSprite(int aCol, int aRow);
	inline SDL_Rect GetSelection() const { return mClip; };

private:

	SDL_Surface* mImage = nullptr;
	SDL_Rect mClip;
};