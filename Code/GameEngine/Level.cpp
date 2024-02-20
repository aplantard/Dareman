#include <cassert>
#include <GameEngine/GameEngine.h>
#include "Renderer.h"
#include "SpriteManager.h"
#include "Dareman.h"
#include "Ghost.h"

#include "Level.h"

Level::Level()
	: mWidth(0)
	, mHeight(0)
	, mPickupCount(0)
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

	char tileId[3] = "00";
	char delimiter;
	mTiles.emplace_back();
	while (fscanf(file, "%2c%c", &tileId, &delimiter) != EOF)
	{
		mTiles.back().emplace_back(Tile(mTileTemplates[tileId]));
		if (delimiter == '\n')
			mTiles.emplace_back();
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
	return mTiles[aRow][aCol];
}

const Tile& Level::GetNextTile(int aCol, int aRow, Direction aDirection) const
{
	switch (aDirection)
	{
	case Up: return GetTile(aCol, aRow - 1);
	case Down: return GetTile(aCol, aRow + 1);
	case Right:
	{
		if (aRow == (mTiles.size()/2) - 1)
		{
			if (aCol == (mTiles[0].size() - 1))
			{
				return GetTile(0, aRow);
			}
		}
		return GetTile(aCol + 1, aRow);
	}
	case Left:
	{
		if (aRow == (mTiles.size() / 2) - 1)
		{
			if (aCol == 0)
			{
				return GetTile(mTiles[0].size() - 1, aRow);
			}
		}

		return GetTile(aCol - 1, aRow);
	}
	default:
		assert(false); // This should not happen
		return mTiles[0][0];
	}
}

std::vector<Direction> Level::ComputeShortestPath(int aStartCol, int aStartRow, int aDestCol, int aDestRow) const
{
	// TODO: Implement A* algorithm here
	// The returned path should only contain the relevant direction changes.
	// Note that ghosts cannot "turn back" and may only choose a direction when several choices are possible
	return {Left};
}
