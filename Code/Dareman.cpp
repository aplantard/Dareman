#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/Level.h>
#include <GameEngine/SpriteSheet.h>
#include <GameEngine/Renderer.h>

#include "Dareman.h"

Dareman::Dareman(int aPosX, int aPosY)
	: mWantedDirection(None)
	, GameActor(aPosX, aPosY)
{
	mDirection = None;

	mSpriteSheet = new SpriteSheet("Data/Spritesheet/pacman.png", 7, 3);
	mSpriteSheet->SelectSprite(0, 2);
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
	switch (mDirection)
	{
		case Up: mSpriteSheet->SelectSprite(2, 2); break;
		case Down: mSpriteSheet->SelectSprite(1, 0); break;
		case Left: mSpriteSheet->SelectSprite(1, 1); break;
		case Right: mSpriteSheet->SelectSprite(0, 2); break;
	}
}

void Dareman::ToggleSprite() 
{
	SDL_Rect selection = mSpriteSheet->GetSelection();

	switch (mDirection)
	{
		case Up:
		{
			if (selection.x/selection.w == 2)
			{
				mSpriteSheet->SelectSprite(1, 2);
			}
			else
			{
				mSpriteSheet->SelectSprite(2, 2);
			}
			break;
		}
		case Down:
		{
			if (selection.x / selection.w == 1)
			{
				mSpriteSheet->SelectSprite(0, 0);
			}
			else
			{
				mSpriteSheet->SelectSprite(1, 0);
			}
			break;
		}
		case Left:
		{
			if (selection.x / selection.w == 1)
			{
				mSpriteSheet->SelectSprite(0, 1);
			}
			else
			{
				mSpriteSheet->SelectSprite(1, 1);
			}
			break;
		}
		case Right:
		{
			if (selection.x / selection.w == 0)
			{
				mSpriteSheet->SelectSprite(2, 1);
			}
			else
			{
				mSpriteSheet->SelectSprite(0, 2);
			}
			break;
		}
	}
}

void Dareman::Update(std::chrono::duration<double, std::milli> aDeltaTime) 
{
	const float deltaSeconds = float(aDeltaTime.count()) / 1000.f;

	if (mDirection == None)
	{
		if (mWantedDirection != None && CanMove(mWantedDirection))
		{
			mDirection = mWantedDirection;
			UpdateSprite();
		}
	}

	// Compute Sprite animation, average calculation to change sprite when changing tile. (It doesn't need to be really accurate).
	{
		mDistanceMoved += (GAMEOBJECT_SPEED / TILE_SIZE) * deltaSeconds;

		if ((int)mDistanceMoved >= 1)
		{
			ToggleSprite();
			mDistanceMoved = 0;
		}
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

void Dareman::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSpriteFromSpriteSheet(mSpriteSheet, mPosX, mPosY);
}
