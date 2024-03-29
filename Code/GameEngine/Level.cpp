#include <cassert>
#include <GameEngine/GameEngine.h>
#include "Renderer.h"
#include "SpriteManager.h"
#include "Dareman.h"
#include "Ghost.h"

#include "Level.h"

// struct only usefull for a* algorithme
struct TileNode
{
	TileNode(const Tile* aTile)
		: mTile(aTile)
	{
	}

	const Tile* mTile = nullptr;
	TileNode* mParent = nullptr;
	int mCost = 0;
	int mHeuristic = 0;

	inline int GetScore() const { return mCost + mHeuristic; };
};

TileNode* FindNodeInList(std::vector<TileNode*>& aNodes, TileNode* aNode)
{
	for (TileNode* node : aNodes)
	{
		if (node->mTile == aNode->mTile)
		{
			return node;
		}
	}
	return nullptr;
}


Level::Level()
	: mWidth(0)
	, mHeight(0)
	, mPickupCount(0)
	, mSmallPickupSprite(nullptr)
	, mBigPickupSprite(nullptr)
	, mTotalPickupCount(0)
{
}

Level::~Level()
{
	for (auto& tile : mTileTemplates)
	{
		delete tile.second.mSprite;
	}
	delete mSmallPickupSprite;
	delete mBigPickupSprite;
}

void Level::LoadTilesSprite() 
{
	SpriteManager* spriteManager = GameEngine::GetInstance()->GetSpriteManager();

	if (spriteManager)
	{
		InitTileTemplates(spriteManager);
		mSmallPickupSprite = spriteManager->LoadSprite("Data/Images/pickup_small.png");
		mBigPickupSprite = spriteManager->LoadSprite("Data/Images/pickup_big.png");
	}
}

void Level::LoadLevel(const char* aPath)
{
	std::string rootPath = aPath;
	LoadLevelFile((rootPath + "Level.txt").c_str());
	LoadPickupFile((rootPath + "Pickups.txt").c_str());
}

void Level::InitTileTemplates(SpriteManager* aSpriteManager)
{
	typedef std::pair<std::string, Tile> PairType;

	mTileTemplates.emplace(PairType("c1", {aSpriteManager->LoadSprite("Data/Images/c1.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("c2", {aSpriteManager->LoadSprite("Data/Images/c2.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("c3", {aSpriteManager->LoadSprite("Data/Images/c3.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("c4", {aSpriteManager->LoadSprite("Data/Images/c4.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("w1", {aSpriteManager->LoadSprite("Data/Images/w1.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("w2", {aSpriteManager->LoadSprite("Data/Images/w2.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("w3", {aSpriteManager->LoadSprite("Data/Images/w3.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("w4", {aSpriteManager->LoadSprite("Data/Images/w4.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("d1", {aSpriteManager->LoadSprite("Data/Images/d1.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("d2", {aSpriteManager->LoadSprite("Data/Images/d2.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("d3", {aSpriteManager->LoadSprite("Data/Images/d3.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("d4", {aSpriteManager->LoadSprite("Data/Images/d4.png"), Collision::CollidesAll, Pickup::None}));

	mTileTemplates.emplace(PairType("ff", {aSpriteManager->LoadSprite("Data/Images/ff.png"), Collision::CollidesAll, Pickup::None}));
	mTileTemplates.emplace(PairType("f0", {nullptr, Collision::CollidesPlayer, Pickup::None}));
	mTileTemplates.emplace(PairType("00", {nullptr, Collision::None, Pickup::None}));

	for (auto& tile : mTileTemplates)
	{
		if (tile.second.mSprite)
			tile.second.mSprite->SetSize(TILE_SIZE, TILE_SIZE);
	}
}

void Level::LoadLevelFile(const char* aPath)
{
	mTiles.clear();

	// The data is expected to be properly formatted
	// All lines should be the same length and consisting of series of two characters separated by spaces
	FILE* file = fopen(aPath, "r");

	int row = 0;
	int col = 0;

	char tileId[3] = "00";
	char delimiter;
	mTiles.emplace_back();
	while (fscanf(file, "%s%c", &tileId, &delimiter) != EOF)
	{
		mTiles.back().emplace_back(Tile(mTileTemplates[tileId]));
		mTiles.back().back().mCol = col;
		mTiles.back().back().mRow = row;
		col++;
		if (delimiter == '\n')
		{
			mTiles.emplace_back();
			col = 0;
			row++;
		}
	}

	fclose(file);

	// Check integrity of the level
	mWidth = (int)mTiles[0].size();
	mHeight = (int)mTiles.size();
}

void Level::LoadPickupFile(const char* aPath)
{
	GameEngine* gameEngine = GameEngine::GetInstance();

	mPickupCount = 0;

	// The data is expected to be properly formatted and fitting the Level.txt
	FILE* file = fopen(aPath, "r");

	char pickupType;
	char delimiter;
	int col = 0;
	int row = 0;

	Dareman* dareman = nullptr;

	while (fscanf(file, "%c%c", &pickupType, &delimiter) != EOF)
	{
		switch (pickupType)
		{
			case '1':
				mPickupCount++;
				mTiles[row][col].mPickup = Pickup::Small;
				break;
			case '2':
				mPickupCount++;
				mTiles[row][col].mPickup = Pickup::Big;
				break;
			case 'D':
			{
				dareman = new Dareman(col, row);
				break;
			}
			case 'B':
			case 'I':
			case 'P':
			case 'C': 
			{
				gameEngine->AddActor(new Ghost((Character)pickupType, col, row));
				break;
			}
			default: break;
		}

		if (delimiter == '\n')
		{
			row++;
			col = 0;
		}
		else
		{
			col++;
		}
	}

	fclose(file);

	gameEngine->AddActor(dareman);

	// Check integrity of the level
	mWidth = (int)mTiles[0].size();
	mHeight = (int)mTiles.size();

	mTotalPickupCount = mPickupCount;
}

bool Level::IsValid() const
{
	return mTiles.size() > 0 && mPickupCount > 0;
}

void Level::Render(Renderer* aRenderer) const
{
	for (int row = 0; row < mHeight; row++)
	{
		for (int col = 0; col < mWidth; col++)
		{
			const auto& tile = mTiles[row][col];
			if (tile.mSprite)
				aRenderer->DrawSprite(tile.mSprite, col * TILE_SIZE, row * TILE_SIZE);
			else
			{
				switch (tile.mPickup)
				{
					case Pickup::Small: aRenderer->DrawSprite(mSmallPickupSprite, col * TILE_SIZE, row * TILE_SIZE); break;
					case Pickup::Big: aRenderer->DrawSprite(mBigPickupSprite, col * TILE_SIZE, row * TILE_SIZE); break;
					default: break;
				}
			}
		}
	}
}

const Tile& Level::GetTile(int aCol, int aRow) const
{
	assert(aRow >= 0 && aRow < mTiles.size());
	assert(aCol >= 0 && aCol < mTiles[aRow].size());

	return mTiles[aRow][aCol];
}

const Tile& Level::GetNextTile(int aCol, int aRow, Direction aDirection) const
{
	int levelWidth = mTiles[0].size();
	int levelHeight = mTiles.size();

	switch (aDirection)
	{
	case Up: return GetTile(aCol, std::max(aRow - 1, 0));
	case Down: return GetTile(aCol, std::min(aRow + 1, levelHeight - 1));
	case Right:
	{
		if (aRow == (levelHeight / 2) - 1)
		{
			if (aCol == (levelWidth - 1))
			{
				return GetTile(0, aRow);
			}
		}
		return GetTile(std::min(aCol + 1, levelWidth - 1), aRow);
	}
	case Left:
	{
		if (aRow == (levelHeight / 2) - 1)
		{
			if (aCol == 0)
			{
				return GetTile(levelWidth - 1, aRow);
			}
		}

		return GetTile(std::max(aCol - 1, 0), aRow);
	}
	default:
		assert(false); // This should not happen
		return mTiles[0][0];
	}
}

// Not optimized, Would need to check how to use this better.
const Tile& Level::GetClosestTileNonBlocking(int aCol, int aRow, Direction aDirection) const
{
	Tile startTile = GetTile(aCol, aRow);

	if (startTile.mCollision != Collision::CollidesAll)
	{
		return startTile;
	}

	std::vector<std::pair<Tile,Direction>> openList;

	Tile frontTile = GetNextTile(startTile.mCol, startTile.mRow, aDirection);
	if (frontTile.mCollision == Collision::CollidesAll && frontTile != startTile)
	{
		openList.push_back(std::pair<Tile,Direction>(frontTile, aDirection));
	}
	else if (frontTile != startTile)
	{
		return frontTile;
	}

	Direction rightDirection = ClockWiseRotate(aDirection);
	Tile rightTile = GetNextTile(startTile.mCol, startTile.mRow, rightDirection);
	if (rightTile.mCollision == Collision::CollidesAll && rightTile != startTile)
	{
		openList.push_back(std::pair<Tile,Direction>(rightTile, rightDirection));
	}
	else if (rightTile != startTile)
	{
		return rightTile;
	}

	Direction leftDirection = AntiClockWiseRotate(aDirection);
	Tile leftTile = GetNextTile(startTile.mCol, startTile.mRow, AntiClockWiseRotate(aDirection));
	if (leftTile.mCollision == Collision::CollidesAll && leftTile != startTile)
	{
		openList.push_back(std::pair<Tile, Direction>(leftTile, leftDirection));
	}
	else if (leftTile != startTile)
	{
		return leftTile;
	}

	Direction behindDirection = GetOppositeDirection(aDirection);
	Tile behindTile = GetNextTile(startTile.mCol, startTile.mRow, GetOppositeDirection(aDirection));
	if (behindTile.mCollision == Collision::CollidesAll && behindTile != startTile)
	{
		openList.push_back(std::pair<Tile, Direction>(behindTile, behindDirection));
	}
	else if (behindTile != startTile)
	{
		return behindTile;
	}

	std::pair<Tile, Direction> currentTile = openList[0];

	while (openList.empty() == false && openList.size() < (mTiles.size() * mTiles[0].size()))
	{
		rightDirection = ClockWiseRotate(currentTile.second);
		rightTile = GetNextTile(currentTile.first.mCol, currentTile.first.mRow, rightDirection);

		if (rightTile.mCollision == Collision::CollidesAll && rightTile != currentTile.first)
		{
			openList.push_back(std::pair<Tile, Direction>(rightTile, rightDirection));
		}
		else if (rightTile != currentTile.first)
		{
			return rightTile;
		}

		leftDirection = AntiClockWiseRotate(currentTile.second);
		leftTile = GetNextTile(currentTile.first.mCol, currentTile.first.mRow, leftDirection);
		if (leftTile.mCollision == Collision::CollidesAll && leftTile != currentTile.first)
		{
			openList.push_back(std::pair<Tile, Direction>(leftTile, leftDirection));
		}
		else if (leftTile != currentTile.first)
		{
			return leftTile;
		}

		
		frontTile = GetNextTile(currentTile.first.mCol, currentTile.first.mRow, currentTile.second);
		if (frontTile.mCollision == Collision::CollidesAll && frontTile != currentTile.first)
		{
			openList.push_back(std::pair<Tile, Direction>(frontTile, currentTile.second));
		}
		else if (frontTile != currentTile.first)
		{
			return frontTile;
		}

		openList.erase(openList.begin());
		currentTile = openList[0];
	}
	openList.clear();
	return startTile;
}

void Level::RemovePickUp(int aCol, int aRow)
{
	if (mTiles[aRow][aCol].mPickup != Pickup::None)
	{
		mTiles[aRow][aCol].mPickup = Pickup::None;
		mPickupCount--;
	}
}

std::vector<Direction> Level::ComputePath(
	int aStartCol, int aStartRow, int aDestCol, int aDestRow, Direction aDirectionFrom) const
{
	std::vector<TileNode*> openList;
	openList.reserve(128);
	std::vector<TileNode*> closedList;
	closedList.reserve(128);

	TileNode* startTile = new TileNode(&GetTile(aStartCol, aStartRow));
	TileNode* previousTile = nullptr;
	if (aDirectionFrom != Direction::None)
	{
		previousTile = new TileNode(&GetNextTile(aStartCol, aStartRow, GetOppositeDirection(aDirectionFrom)));
	}
	TileNode* destTile = new TileNode(&GetTile(aDestCol, aDestRow));
	TileNode* currentTile = nullptr;

	openList.push_back(startTile);

	while (openList.empty() == false)
	{
		auto current_it = openList.begin();
		currentTile = *current_it;

		for (auto it = openList.begin(); it != openList.end(); it++)
		{
			TileNode* tile = *it;
			if (tile->GetScore() <= currentTile->GetScore())
			{
				currentTile = tile;
				current_it = it;
			}
		}


		if (currentTile->mTile == destTile->mTile)
		{
			break;
		}

		closedList.push_back(currentTile);
		openList.erase(current_it);
			
		for (int i = 0; i < Direction::None; ++i)
		{
			Direction currentDir = static_cast<Direction>(i);
			TileNode* nextTile = new TileNode(&GetNextTile(currentTile->mTile->mCol, currentTile->mTile->mRow, currentDir));
			if (currentTile->mTile == startTile->mTile && previousTile != nullptr && nextTile->mTile == previousTile->mTile)
			{
				delete nextTile;
				nextTile = nullptr;

				continue;
			}

			if (nextTile->mTile->mCollision != Collision::CollidesAll && FindNodeInList(closedList, nextTile) == nullptr)
			{
				int totalCost = currentTile->mCost + 1;

				TileNode* successor = FindNodeInList(openList, nextTile);

				if (successor == nullptr)
				{
					successor = nextTile;
					successor->mParent = currentTile;
					successor->mCost = totalCost;
					successor->mHeuristic = GetManhattanDistance(successor->mTile->mCol, successor->mTile->mRow, aDestCol, aDestRow);
					openList.push_back(successor);
				}
				else if (totalCost < successor->mCost)
				{
					successor->mParent = currentTile;
					successor->mCost = totalCost;

					delete nextTile;
					nextTile = nullptr;
				}
				else
				{
					delete nextTile;
					nextTile = nullptr;
				}
			}
			else
			{
				delete nextTile;
				nextTile = nullptr;
			}
		}
	}

	std::vector<Direction> toReturn;

	while (currentTile->mParent != nullptr)
	{
		toReturn.push_back(GetDirectionToMove(currentTile->mParent->mTile->mCol, currentTile->mParent->mTile->mRow, currentTile->mTile->mCol, currentTile->mTile->mRow));
		currentTile = currentTile->mParent;
	}


	for (auto it = openList.begin(); it != openList.end();)
	{
		delete *it;
		it = openList.erase(it);
	}

	for (auto it = closedList.begin(); it != closedList.end();)
	{
		delete *it;
		it = closedList.erase(it);
	}

	delete destTile;
	destTile = nullptr;

	delete previousTile;
	previousTile = nullptr;

	return toReturn;
}

float Level::GetManhattanDistance(int aStartCol, int aStartRow, int aDestCol, int aDestRow) const
{
	return abs(aDestCol - aStartCol) + abs(aDestRow - aStartRow);
}

Direction Level::GetDirectionToMove(int aFromCol, int aFromRow, int aToCol, int aToRow) const
{
	int diffCol = aToCol - aFromCol;
	int diffRow = aToRow - aFromRow;

	if (diffCol == 0 && diffRow == -1)
	{
		return Direction::Up;
	}
	else if (diffCol == 0 && diffRow == 1)
	{
		return Direction::Down;
	}
	else if (diffCol == 1 && diffRow == 0)
	{
		return Direction::Right;
	}
	else if (diffCol == -1 && diffRow == 0)
	{
		return Direction::Left;
	}

	// manage the corridor
	int middleRow = ((GetHeightPx() / TILE_SIZE) / 2) - 1;
	if ((aFromRow == middleRow && aToRow == middleRow))
	{
		int lastCol = (GetWidthPx() / TILE_SIZE) - 1;
		if (aFromCol == 0 && aToCol == lastCol)
		{
			return Direction::Left;
		}
		else if (aFromCol == lastCol && aToCol == 0)
		{
			return Direction::Right;
		}
	}

	return Direction::None;
}

Direction Level::GetOppositeDirection(Direction aDirection) const
{
	switch (aDirection)
	{
		case Direction::Up: return Direction::Down;
		case Direction::Down: return Direction::Up;
		case Direction::Right: return Direction::Left;
		case Direction::Left: return Direction::Right;
		case Direction::None: return Direction::Right;
		default:
		{
			assert(false); // This should not happen
			return Direction::None;
		}
	}
}

Direction Level::ClockWiseRotate(Direction aDirection) const
{
	switch (aDirection)
	{
	case Direction::Up: return Direction::Right;
	case Direction::Down: return Direction::Left;
	case Direction::Right: return Direction::Down;
	case Direction::Left: return Direction::Up;
	case Direction::None: return Direction ::Up;
	default:
	{
		assert(false); // This should not happen
		return Direction::None;
	}
	}
}

Direction Level::AntiClockWiseRotate(Direction aDirection) const
{
	switch (aDirection)
	{
	case Direction::Up: return Direction::Left;
	case Direction::Down: return Direction::Right;
	case Direction::Right: return Direction::Up;
	case Direction::Left: return Direction::Down;
	case Direction::None: return Direction ::Up;
	default:
	{
		assert(false); // This should not happen
		return Direction::None;
	}
	}
}
