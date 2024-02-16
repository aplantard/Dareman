#include "Dareman.h"

#include <cassert>

#include "Level.h"
#include "Renderer.h"
#include "Sprite.h"
#include "SpriteManager.h"

Dareman::Dareman()
	: mCurrentSprite(SpriteManager::GetInstance().GetSprite("Data/Images/pac_r2.png"))
	, mPosX(0)
	, mPosY(0)
	, mDirection(None)
	, mWantedDirection(None)
{
}

Dareman::~Dareman() {}

void Dareman::SetPosition(int aPosX, int aPosY)
{
	mPosX = (float)aPosX;
	mPosY = (float)aPosY;
}

bool Dareman::CanMove(Direction aDirection) const
{
	assert(IsOnTile());
	return Level::GetInstance().GetNextTile((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, aDirection).mCollision == Collision::None;
}

bool Dareman::IsOnTile() const
{
	return int(mPosX) % TILE_SIZE == 0 && int(mPosY) % TILE_SIZE == 0;
}

float Dareman::MoveToNextTile(float aDeltaTime)
{
	const float distanceMax = aDeltaTime * GAMEOBJECT_SPEED;

	// Move to next tile, or finish moving into tile in progress
	switch (mDirection)
	{
	case Up:
	{
		int nextTileY = (int)mPosY / TILE_SIZE * TILE_SIZE;
		if (nextTileY == (int)mPosY)
			nextTileY = (int)mPosY - TILE_SIZE;

		const float distanceToNextTile = mPosY - nextTileY;
		if (distanceMax > distanceToNextTile)
		{
			mPosY = (float)nextTileY;
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosY -= distanceMax;
			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
		}
	}
	break;
	case Down:
	{
		const int nextTileY = ((int)mPosY + TILE_SIZE) / TILE_SIZE * TILE_SIZE;
		const float distanceToNextTile = nextTileY - mPosY;
		if (distanceMax > distanceToNextTile)
		{
			mPosY = (float)nextTileY;
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosY += distanceMax;
			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
		}
	}
	break;
	case Right:
	{
		const int nextTileX = ((int)mPosX + TILE_SIZE) / TILE_SIZE * TILE_SIZE;
		const float distanceToNextTile = nextTileX - mPosX;
		if (distanceMax > distanceToNextTile)
		{
			mPosX = (float)nextTileX;
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosX += distanceMax;
			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
		}
	}
	break;
	case Left:
	{
		int nextTileX = (int)mPosX / TILE_SIZE * TILE_SIZE;
		if (nextTileX == (int)mPosX)
			nextTileX = (int)mPosX - TILE_SIZE;

		const float distanceToNextTile = mPosX - nextTileX;

		if (distanceMax > distanceToNextTile)
		{
			mPosX = (float)nextTileX;
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosX -= distanceMax;
			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
		}
	}
	break;
	default: assert(false && "Needs a direction to move"); break;
	}

	return aDeltaTime;
}

void Dareman::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSprite(mCurrentSprite, (int)mPosX, (int)mPosY);
}

void Dareman::UpdateSprite()
{
	switch (mDirection)
	{
	case Up: mCurrentSprite = SpriteManager::GetInstance().GetSprite("Data/Images/pac_u2.png"); break;
	case Down: mCurrentSprite = SpriteManager::GetInstance().GetSprite("Data/Images/pac_d2.png"); break;
	case Left: mCurrentSprite = SpriteManager::GetInstance().GetSprite("Data/Images/pac_l2.png"); break;
	case Right: mCurrentSprite = SpriteManager::GetInstance().GetSprite("Data/Images/pac_r2.png"); break;
	}
}
