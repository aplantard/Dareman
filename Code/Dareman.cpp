#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/Level.h>
#include <GameEngine/SpriteSheet.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/GameStateMgr.h>

#include "Dareman.h"

Dareman::Dareman(int aCol, int aRow)
	: mWantedDirection(None)
	, GameActor(aCol, aRow)
{
	mDirection = None;

	mSpriteSheet = new SpriteSheet("Data/Spritesheet/pacman.png", 7, 3);
	mSpriteSheet->SelectSprite(0, 2);
}

Dareman::~Dareman() 
{
	delete mSpriteSheet;
	mSpriteSheet = nullptr;
}

void Dareman::SetWantedDirection(Direction aNewDirection) 
{
	mWantedDirection = aNewDirection;
}

// Used to change sprite according to direction
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

// It is use for animation when we stay in the same direction
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

void Dareman::DaremanDeathAnimation(std::chrono::duration<double, std::milli> aDeltaTime) 
{
	mDeathAnimationStepDuration += aDeltaTime.count();

	if (mDeathAnimationStepDuration >= mDeathAnimationTotalDuration/9.f)
	{
		mDeathAnimationStepDuration = 0;
		mDeathAnimationStepPassed++;

		mSpriteSheet->SelectSprite(mDeathAnimationStepPassed%3, 3 + (mDeathAnimationStepPassed/3));
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
		mDistanceMoved += (mSpeed / TILE_SIZE) * deltaSeconds;

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
				break;
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

bool Dareman::CanMove(Direction aDirection) const
{
	assert(IsOnTile());
	return GameEngine::GetInstance()->GetLevel()->GetNextTile((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, aDirection).mCollision
		== Collision::None;
}