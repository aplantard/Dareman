#pragma once

#include <map>
#include <string>
#include <vector>

#include "Dareman.h"

#define TILE_SIZE 24

class Sprite;
class Renderer;
class SpriteManager;

enum class Collision : char
{
	None,
	CollidesAll,
	CollidesPlayer
};

enum class Pickup : char
{
	None,
	Small,
	Big
};

enum class Character : char
{
	Dareman = 'D',
	Blinky = 'B',
	Inky = 'I',
	Pinky = 'P',
	Clyde = 'C'
};

struct Tile
{
	Sprite* mSprite = nullptr;
	Collision mCollision = Collision::None;
	Pickup mPickup = Pickup::None;
};

class Level
{
public:
	Level();
	~Level();

	void LoadTilesSprite();
	void LoadLevel(const char* aPath);
	bool IsValid() const;

	void Render(Renderer* aRenderer) const;

	const Tile& GetTile(int aCol, int aRow) const;
	const Tile& GetNextTile(int aCol, int aRow, Direction aDirection) const;
	int GetPickupCount() const { return mPickupCount; }

	std::vector<Direction> ComputeShortestPath(int aStartCol, int aStartRow, int aDestCol, int aDestRow) const;

	int GetWidthPx() const { return mWidth * TILE_SIZE; }
	int GetHeightPx() const { return mHeight * TILE_SIZE; }

private:
	void InitTileTemplates(SpriteManager* aSpriteManager);
	void LoadLevelFile(const char* aPath);
	void LoadPickupFile(const char* aPath);


	std::vector<std::vector<Tile>> mTiles;
	std::map<std::string, Tile> mTileTemplates;
	Sprite* mSmallPickupSprite;
	Sprite* mBigPickupSprite;
	int mWidth;
	int mHeight;
	int mPickupCount;
};