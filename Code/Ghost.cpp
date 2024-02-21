#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/SpriteSheet.h>
#include <GameEngine/Level.h>
 
#include "Ghost.h"



Ghost::Ghost(Character aCharacter, int aPosX, int aPosY)
	: mCharacter(aCharacter)
	, GameActor(aPosX,aPosY)
{
	mSpriteSheet = new SpriteSheet("Data/Spritesheet/ghosts.png", 20, 2);
	mSpriteSheet->SelectSprite(0, 2);
	switch (aCharacter)
	{
	case Character::Blinky: mSpriteSheet->SelectSprite(0, 0); break;
	case Character::Inky: mSpriteSheet->SelectSprite(0, 8); break;
	case Character::Pinky: mSpriteSheet->SelectSprite(0, 12); break;
	case Character::Clyde: mSpriteSheet->SelectSprite(0, 4); break;
	default: assert(false && "This character is not a ghost");
	}

	mDirection = Up;
}

Ghost::~Ghost()
{
	delete mSpriteSheet;
	mSpriteSheet = nullptr;
}

void Ghost::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSpriteFromSpriteSheet(mSpriteSheet, mPosX, mPosY);
}

void Ghost::Update(std::chrono::duration<double, std::milli> aDeltaTime) 
{
	const float deltaSeconds = float(aDeltaTime.count()) / 1000.f;

	float remaining = deltaSeconds;
	while (remaining > 0.f)
	{
		if (IsOnTile())
		{
			GameEngine* gameEngine = GameEngine::GetInstance();
			Level* level = gameEngine->GetLevel();
			std::pair<float, float> daremanPos = gameEngine->GetDareman()->GetPosition();

			/* if (GetNumberOfPathAvailableFromPos((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, mDirection) > 1)
			{
				mDirections = level->ComputeShortestPath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, (int)daremanPos.first / TILE_SIZE,
					(int)daremanPos.second / TILE_SIZE, mDirection);
				mDirection = mDirections[0];
			}
			else
			{*/
				
				if (mDirections.empty())
				{
					mDirections = level->ComputeShortestPath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, (int)daremanPos.first / TILE_SIZE,
						(int)daremanPos.second / TILE_SIZE, mDirection);
				}
				else
				{
					mDirections.erase(mDirections.begin());
				}
				mDirection = mDirections[0];

			//}

			remaining = MoveToNextTile(remaining);
		}
		else
		{
			remaining = MoveToNextTile(remaining);
		}
	}
}

int Ghost::GetNumberOfPathAvailableFromPos(int aCol, int aRow, Direction aDirectionFrom)
{
	Level* level = GameEngine::GetInstance()->GetLevel();
	int numberOfPathAvailable = 0;

	for (int i = 0; i < Direction::None; ++i)
	{
		Direction currentDir = static_cast<Direction>(i);

		if (currentDir != level->GetOppositeDirection(aDirectionFrom))
		{
			if (level->GetNextTile(aCol, aRow, currentDir).mCollision != Collision::CollidesAll)
			{
				numberOfPathAvailable++;
			}
		}
	}

	return numberOfPathAvailable;
}
