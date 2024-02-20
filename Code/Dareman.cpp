#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/Level.h>

#include "Dareman.h"

Dareman::Dareman(int aPosX, int aPosY)
	: mWantedDirection(None)
	, GameActor(aPosX, aPosY)
{
	mDirection = None;

	mSprite = GameEngine::GetInstance()->GetSpriteManager()->GetSprite("Data/Images/pac_r2.png");
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
	return GameEngine::GetInstance()->GetLevel()->GetNextTile((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, aDirection).mCollision == Collision::None;
}

bool Dareman::IsOnTile() const
{
	return int(mPosX) % TILE_SIZE == 0 && int(mPosY) % TILE_SIZE == 0;
}

float Dareman::MoveToNextTile(float aDeltaTime)
{
	const float distanceMax = aDeltaTime * GAMEOBJECT_SPEED;

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
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosX += distanceMax;

			if (mPosX >= levelWidth)
			{
				mPosX = 0;
			}
			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
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
			aDeltaTime -= distanceToNextTile / GAMEOBJECT_SPEED;
		}
		else
		{
			mPosX -= distanceMax;

			if (mPosX <= 0)
			{
				mPosX = levelWidth - TILE_SIZE;
			}

			aDeltaTime -= distanceMax / GAMEOBJECT_SPEED;
		}
	}
	break;
	default: assert(false && "Needs a direction to move"); break;
	}

	return aDeltaTime;
}

void Dareman::SetWantedDirection(Direction aNewDirection) 
{
	mWantedDirection = aNewDirection;
}


void Dareman::UpdateSprite()
{
	SpriteManager* spriteManager = GameEngine::GetInstance()->GetSpriteManager();
	if (spriteManager)
	{
		switch (mDirection)
		{
		case Up: mSprite = spriteManager->GetSprite("Data/Images/pac_u2.png"); break;
		case Down: mSprite = spriteManager->GetSprite("Data/Images/pac_d2.png"); break;
		case Left: mSprite = spriteManager->GetSprite("Data/Images/pac_l2.png"); break;
		case Right: mSprite = spriteManager->GetSprite("Data/Images/pac_r2.png"); break;
		}
	}
}

void Dareman::Update(std::chrono::duration<double, std::milli> aDeltaTime) 
{
	const float deltaSeconds = float(aDeltaTime.count()) / 1000.f;

	if (mDirection == None)
	{
		mDirection = mWantedDirection;
		UpdateSprite();
	}

	float remaining = deltaSeconds;
	while (remaining > 0.f && mDirection != None)
	{
		if (IsOnTile())
		{
			GameEngine::GetInstance()->EatPickUp((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE);

			if ((mWantedDirection != None && mWantedDirection != mDirection) && CanMove(mWantedDirection))
			{
				mDirection = mWantedDirection;
				UpdateSprite();
			}

			if (CanMove(mDirection))
			{
				remaining = MoveToNextTile(remaining);
			}
			else
			{
				mDirection = None; // Stop movement and exit loop
			}
		}
		else
		{
			remaining = MoveToNextTile(remaining);
		}
	}
}
