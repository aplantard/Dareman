#pragma once

#include <utility>

#include "Dareman.h"
#include "Level.h"

class Renderer;

class Ghost
{
public:
	Ghost(Character aCharacter);

	void Render(Renderer* aRenderer) const;

private:
	void SetTilePosition(const std::pair<int, int>& aPosition);

	Character mCharacter;
	Sprite* mSprite;
	Direction mDirection;
	float mPosX;
	float mPosY;
};