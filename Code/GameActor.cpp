#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>

#include "GameActor.h"

GameActor::GameActor(int aCol, int aRow)
	: mSprite(nullptr)
	, mDirection(None)
	, mSpeed(TILE_SIZE * 6)
{
	SetTilePosition(aCol, aRow);
}

GameActor::~GameActor() {}

void GameActor::SetTilePosition(int aCol, int aRow)
{
	mPosX = float(aCol * TILE_SIZE);
	mPosY = float(aRow * TILE_SIZE);
}

void GameActor::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSprite(mSprite, (int)mPosX, (int)mPosY);
}

bool GameActor::IsOnTile() const
{
	return int(mPosX) % TILE_SIZE == 0 && int(mPosY) % TILE_SIZE == 0;
}

float GameActor::MoveToNextTile(float aDeltaTime)
{
	const float distanceMax = aDeltaTime * mSpeed;

	Level* level = GameEngine::GetInstance()->GetLevel();

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
			aDeltaTime -= distanceToNextTile / mSpeed;
		}
		else
		{
			mPosY -= distanceMax;
			aDeltaTime -= distanceMax / mSpeed;
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
			aDeltaTime -= distanceToNextTile / mSpeed;
		}
		else
		{
			mPosY += distanceMax;
			aDeltaTime -= distanceMax / mSpeed;
		}
	}
	break;
	case Right:
	{
		int levelWidth = level->GetWidthPx();

		int nextTileX = ((int)mPosX + TILE_SIZE) / TILE_SIZE * TILE_SIZE;

		if (nextTileX >= levelWidth)
		{
			nextTileX = 0;
		}

		float distanceToNextTile = nextTileX - mPosX;

		if (distanceToNextTile < 0)
		{
			distanceToNextTile = (levelWidth - mPosX);
		}

		if (distanceMax > distanceToNextTile)
		{
			mPosX = (float)nextTileX;
			aDeltaTime -= distanceToNextTile / mSpeed;
		}
		else
		{
			mPosX += distanceMax;

			if (mPosX >= levelWidth)
			{
				mPosX = 0;
			}
			aDeltaTime -= distanceMax / mSpeed;
		}
	}
	break;
	case Left:
	{
		int levelWidth = level->GetWidthPx();
		int nextTileX = (int)mPosX / TILE_SIZE * TILE_SIZE;

		if (nextTileX == (int)mPosX)
		{
			if (nextTileX <= 0)
			{
				nextTileX = (levelWidth - TILE_SIZE);
			}
			else
			{
				nextTileX = (int)mPosX - TILE_SIZE;
			}
		}

		float distanceToNextTile = mPosX - nextTileX;

		if (distanceToNextTile < 0)
		{
			distanceToNextTile = (levelWidth - std::abs(distanceToNextTile));
		}

		if (distanceMax > distanceToNextTile)
		{
			mPosX = (float)nextTileX;
			aDeltaTime -= distanceToNextTile / mSpeed;
		}
		else
		{
			mPosX -= distanceMax;

			if (mPosX <= 0)
			{
				mPosX = levelWidth - TILE_SIZE;
			}

			aDeltaTime -= distanceMax / mSpeed;
		}
	}
	break;
	default: assert(false && "Needs a direction to move"); break;
	}

	return aDeltaTime;
}