#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/SpriteSheet.h>
#include <GameEngine/Level.h>
#include <Dareman.h>
 
#include "Ghost.h"

constexpr auto BLINKY_ROW = 0;
constexpr auto CLYDE_ROW = 4;
constexpr auto INKY_ROW = 8;
constexpr auto PINKY_ROW = 12;

Ghost::Ghost(Character aCharacter, int aPosX, int aPosY)
	: mCharacter(aCharacter)
	, mState(GhostState::Chasing)
	, GameActor(aPosX,aPosY)
{
	mSpriteSheet = new SpriteSheet("Data/Spritesheet/ghosts.png", 20, 2);
	mSpriteSheet->SelectSprite(0, 2);
	switch (aCharacter)
	{
	case Character::Blinky: mSpriteSheet->SelectSprite(0, BLINKY_ROW); break;
	case Character::Inky: mSpriteSheet->SelectSprite(0, INKY_ROW); break;
	case Character::Pinky: mSpriteSheet->SelectSprite(0, PINKY_ROW); break;
	case Character::Clyde: mSpriteSheet->SelectSprite(0, CLYDE_ROW); break;
	default: assert(false && "This character is not a ghost");
	}

	mDirection = Up;
	mDirections.reserve(128);
}

Ghost::~Ghost()
{
	delete mSpriteSheet;
	mSpriteSheet = nullptr;
}

void Ghost::ClearDirections()
{
	mDirections.clear();
}

void Ghost::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSpriteFromSpriteSheet(mSpriteSheet, mPosX, mPosY);
}

void Ghost::Update(std::chrono::duration<double, std::milli> aDeltaTime) 
{
	const float deltaSeconds = float(aDeltaTime.count()) / 1000.f;

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
			GameEngine* gameEngine = GameEngine::GetInstance();
			Level* level = gameEngine->GetLevel();
			std::pair<float, float> daremanPos = gameEngine->GetDareman()->GetPosition();

			if (mState != GhostState:: Recovering && GetNumberOfPathAvailableFromPos((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, mDirection) > 1)
			{
				if (mState == GhostState::Fleeing)
				{
					mDirections = level->ComputePath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE,
						(int)daremanPos.first / TILE_SIZE, (int)daremanPos.second / TILE_SIZE, mDirection, false);
				}
				else if (mState == GhostState::Chasing)
				{
					mDirections = level->ComputePath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, (int)daremanPos.first / TILE_SIZE,
						(int)daremanPos.second / TILE_SIZE, mDirection, true);
				}

				if (mDirections.empty() == false)
				{
					mDirection = mDirections.back();
					mDirections.pop_back();
				}
			}
			else
			{
				if (mDirections.empty())
				{
					if (mState == GhostState::Fleeing)
					{
						mDirections = level->ComputePath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, (int)daremanPos.first / TILE_SIZE,
							(int)daremanPos.second / TILE_SIZE, mDirection, false);
					}
					else if (mState == GhostState::Chasing)
					{
						mDirections = level->ComputePath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, (int)daremanPos.first / TILE_SIZE,
							(int)daremanPos.second / TILE_SIZE, mDirection, true);
					}
					else if (mState == GhostState::Recovering)
					{
						mDirections = level->ComputePath((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, 13, 15, Direction::None, true);
					}

					if (mDirections.empty() == false)
					{
						mDirection = mDirections.back();
						mDirections.pop_back();
					}
				}
				else
				{
					mDirection = mDirections.back();
					mDirections.pop_back();
				}
			}

			if (mDirection != Direction::None && CanMove(mDirection))
			{
				remaining = MoveToNextTile(remaining);
				UpdateSprite();
			}
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

void Ghost::UpdateSprite() 
{
	if (mState == GhostState::Recovering)
	{	
		switch (mDirection)
		{
		case Up: mSpriteSheet->SelectSprite(1, 17); break;
		case Down: mSpriteSheet->SelectSprite(0, 16); break;
		case Left: mSpriteSheet->SelectSprite(1, 16); break;
		case Right: mSpriteSheet->SelectSprite(0, 17); break;
		}
	}
	else if (mState == GhostState::Chasing)
	{
		int row = 0;

		switch (mCharacter)
		{
		case Character::Blinky: row = BLINKY_ROW; break;
		case Character::Inky: row = INKY_ROW; break;
		case Character::Pinky: row = PINKY_ROW; break;
		case Character::Clyde: row = CLYDE_ROW; break;
		default: break;
		}

		switch (mDirection)
		{
		case Up: mSpriteSheet->SelectSprite(0, row + 3); break;
		case Down: mSpriteSheet->SelectSprite(0, row); break;
		case Left: mSpriteSheet->SelectSprite(0, row + 1); break;
		case Right: mSpriteSheet->SelectSprite(0, row + 2); break;
		}
	}
}

void Ghost::ToggleSprite() 
{
	SDL_Rect selection = mSpriteSheet->GetSelection();
	int col = selection.x / selection.w;

	if (col == 0)
	{
		col = 1;
	}
	else if (col == 1)
	{
		col = 0;
	}
	
	GameEngine* gameEngine = GameEngine::GetInstance();

	if (mState == GhostState::Fleeing && gameEngine->IsWarningEnergizerEnd() == false)
	{
		mSpriteSheet->SelectSprite(col, 18);
	}
	else if (mState == GhostState::Fleeing && gameEngine->IsWarningEnergizerEnd())
	{
		int row = selection.y / selection.h;

		if (col == 0 && row == 18)
		{
			row = 19;
		}
		else if (col == 0 && row == 19)
		{
			row = 18;
		}

		mSpriteSheet->SelectSprite(col, row);
	}
	else if (mState == GhostState::Chasing)
	{
		int row = 0;

		switch (mCharacter)
		{
			case Character::Blinky: row = BLINKY_ROW; break;
			case Character::Inky: row = INKY_ROW; break;
			case Character::Pinky: row = PINKY_ROW; break;
			case Character::Clyde: row = CLYDE_ROW; break;
			default: break;
		}

		switch (mDirection)
		{
			case Up: mSpriteSheet->SelectSprite(col, row + 3); break;
			case Down: mSpriteSheet->SelectSprite(col, row); break;
			case Left: mSpriteSheet->SelectSprite(col, row + 1); break;
			case Right: mSpriteSheet->SelectSprite(col, row + 2); break;
			default: break;
		}
	}
}

bool Ghost::CanMove(Direction aDirection) const
{
	assert(IsOnTile());
	return GameEngine::GetInstance()->GetLevel()->GetNextTile((int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, aDirection).mCollision
		!= Collision::CollidesAll;
}
