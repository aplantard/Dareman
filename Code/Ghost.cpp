#include "Ghost.h"

#include <cassert>

#include "Renderer.h"
#include "Sprite.h"
#include "SpriteManager.h"

Ghost::Ghost(Character aCharacter)
	: mCharacter(aCharacter)
	, mPosX(0)
	, mPosY(0)
	, mDirection(Up)
{
	const char* spritePath = nullptr;
	switch (aCharacter)
	{
	case Character::Blinky: spritePath = "Data/Images/blinky.png"; break;
	case Character::Inky: spritePath = "Data/Images/inky.png"; break;
	case Character::Pinky: spritePath = "Data/Images/pinky.png"; break;
	case Character::Clyde: spritePath = "Data/Images/clyde.png"; break;
	default: assert(false && "This character is not a ghost");
	}

	mSprite = SpriteManager::GetInstance().GetSprite(spritePath);
	SetTilePosition(Level::GetInstance().GetStartPosition(mCharacter));
}

void Ghost::SetTilePosition(const std::pair<int, int>& aPosition)
{
	mPosX = float(aPosition.first * TILE_SIZE);
	mPosY = float(aPosition.second * TILE_SIZE);
}

void Ghost::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSprite(mSprite, (int)mPosX, (int)mPosY);
}
