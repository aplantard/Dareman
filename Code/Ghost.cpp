#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/SpriteSheet.h>
#include <GameEngine/Level.h>
#include <Dareman.h>
#include <Windows.h>
 
#include "Ghost.h"

constexpr auto BLINKY_ROW = 0;
constexpr auto CLYDE_ROW = 4;
constexpr auto INKY_ROW = 8;
constexpr auto PINKY_ROW = 12;

Ghost::Ghost(Character aCharacter, int aPosX, int aPosY)
	: mCharacter(aCharacter)
	, mState(GhostState::Scatter)
	, GameActor(aPosX,aPosY)
{
	mSpriteSheet = new SpriteSheet("Data/Spritesheet/ghosts.png", 20, 2);
	mSpriteSheet->SelectSprite(0, 2);
	mScatterCount++;

	Level* level = GameEngine::GetInstance()->GetLevel();

	switch (aCharacter)
	{
	case Character::Blinky:
	{
		mDefaultTargetTile = level->GetTile(26, 1);
		mSpriteSheet->SelectSprite(0, BLINKY_ROW);
		break;
	}
	case Character::Inky:
	{
		mDefaultTargetTile = level->GetTile(26, 29);
		mSpriteSheet->SelectSprite(0, INKY_ROW);
		break;
	}
	case Character::Pinky:
	{
		mDefaultTargetTile = level->GetTile(1, 1);
		mSpriteSheet->SelectSprite(0, PINKY_ROW);
		break;
	}
	case Character::Clyde:
	{
		mDefaultTargetTile = level->GetTile(1, 29);
		mSpriteSheet->SelectSprite(0, CLYDE_ROW);
		break;
	}
	default: assert(false && "This character is not a ghost");
	}

	mTartgetTile = mDefaultTargetTile;
	mPreviousTile = level->GetTile(mPosX / TILE_SIZE, mPosY / TILE_SIZE);
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

	// Manage Scatter Chasing state switch 
	{
		if (mState == GhostState::Scatter || mState == GhostState::Chasing)
		{
			mChangeStateDuration += aDeltaTime.count();

			if (mState == GhostState::Scatter)
			{
				float scatterDuration = mScatterTotalDuration;
				if (mScatterCount > 2)
				{
					scatterDuration *= 1.4;
				}

				if (mChangeStateDuration >= scatterDuration)
				{
					GameEngine* gameEngine = GameEngine::GetInstance();
					Level* level = gameEngine->GetLevel();

					std::pair<float, float> daremanPos = gameEngine->GetDareman()->GetPosition();

					mTartgetTile = level->GetTile((int)daremanPos.first / TILE_SIZE, (int)daremanPos.second / TILE_SIZE);
					mChangeStateDuration = 0;
					mState = GhostState::Chasing;
					mDirections.clear();
				}
			}
			else if (mScatterCount <= mNbScatterMax)
			{
				if (mChangeStateDuration >= mChasingTotalDuration)
				{
					Level* level = GameEngine::GetInstance()->GetLevel();

					mTartgetTile = mDefaultTargetTile;
					mChangeStateDuration = 0;
					mState = GhostState::Scatter;
					mScatterCount++;
					mDirections.clear();
				}
			}
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
		bool hasComputedPath = false;

		if (IsOnTile())
		{
			GameEngine* gameEngine = GameEngine::GetInstance();
			Level* level = gameEngine->GetLevel();
			std::pair<float, float> daremanPos = gameEngine->GetDareman()->GetPosition();
			Tile currentTile = level->GetTile(mPosX / TILE_SIZE, mPosY / TILE_SIZE);
			int numberOfdirectionAvailable = GetNumberOfPathAvailableFromPos(currentTile.mCol, currentTile.mRow, mDirection);

			//Compute Next direction
			{
				if ((mState == GhostState::Chasing || mState == GhostState::Fleeing) && numberOfdirectionAvailable > 1)
				{
					if (mState == GhostState::Fleeing)
					{
						float maxDistance = 0;
						for (int i = 0; i < Direction::None; ++i)
						{
							Direction currentDir = static_cast<Direction>(i);
							Tile nextTile = level->GetNextTile(currentTile.mCol, currentTile.mRow, currentDir);

							if (level->GetManhattanDistance(
									daremanPos.first / TILE_SIZE, daremanPos.second / TILE_SIZE, nextTile.mCol, nextTile.mRow)
								> maxDistance)
							{
								mDirection = currentDir;
							}
						}
					}
					else if (mState == GhostState::Chasing)
					{
						mTartgetTile = level->GetTile((int)daremanPos.first / TILE_SIZE, (int)daremanPos.second / TILE_SIZE);

						mDirections = level->ComputePath(
							(int)mPosX / TILE_SIZE, (int)mPosY / TILE_SIZE, mTartgetTile.mCol, mTartgetTile.mRow, mDirection);
						hasComputedPath = true;
					}

					if (mDirections.empty() == false)
					{
						mDirection = mDirections.back();
					}
				}
				else
				{
					if (mDirections.empty())
					{
						if (mState == GhostState::Fleeing)
						{
							mDirections = level->ComputePath(currentTile.mCol, currentTile.mRow,
								(int)daremanPos.first / TILE_SIZE, (int)daremanPos.second / TILE_SIZE, Direction::None);
						}
						else if (mState == GhostState::Chasing)
						{
							mTartgetTile = level->GetTile((int)daremanPos.first / TILE_SIZE, (int)daremanPos.second / TILE_SIZE);
							mDirections = level->ComputePath(currentTile.mCol, currentTile.mRow, mTartgetTile.mCol, mTartgetTile.mRow, Direction::None);
						}
						else if (mState == GhostState::Scatter)
						{
							mTartgetTile = mDefaultTargetTile;
							mDirections = level->ComputePath(
								currentTile.mCol, currentTile.mRow, mTartgetTile.mCol, mTartgetTile.mRow, Direction::None);

							char buffer[50];
							sprintf(buffer, "Recompute on tile : [ %d , %d ]\n", currentTile.mCol, currentTile.mRow);
							OutputDebugString(buffer);
						}
						else if (mState == GhostState::Recovering)
						{
							mDirections = level->ComputePath(currentTile.mCol, currentTile.mRow, 13, 15, Direction::None);
						}

						hasComputedPath = true;

						if (mDirections.empty() == false)
						{
							mDirection = mDirections.back();
						}
					}
					else
					{
						mDirection = mDirections.back();
					}
				}

			}

			if ((mPreviousTile != currentTile) && hasComputedPath == false)
			{
				if (mDirections.size() <= 1)
				{
					mDirections.pop_back();
				}
				else
				{
					mDirections.pop_back();
					mDirection = mDirections.back();
				}

				mPreviousTile = currentTile;
			}
			else if (mPreviousTile != currentTile)
			{
				mPreviousTile = currentTile;
			}

			if (mDirection != Direction::None && CanMove(mDirection))
			{
				remaining = MoveToNextTile(remaining);
				UpdateSprite();
			}
			else
			{
				int test = 0;
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
	Tile previousTile;

	if (aDirectionFrom != Direction::None)
	{
		previousTile = level->GetNextTile(aCol, aRow, level->GetOppositeDirection(aDirectionFrom));
	}

	for (int i = 0; i < Direction::None; ++i)
	{
		Direction currentDir = static_cast<Direction>(i);
		Tile currentTile = level->GetNextTile(aCol, aRow, currentDir);
		if (currentTile != previousTile)
		{
			if (currentTile.mCollision != Collision::CollidesAll)
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
	else if (mState == GhostState::Chasing || mState == GhostState::Scatter)
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
	else if (mState == GhostState::Chasing || mState == GhostState::Scatter)
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
