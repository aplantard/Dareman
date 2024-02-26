#pragma once

#include <map>
#include <string>
#include <vector>

constexpr auto TILE_SIZE = 24;

class Sprite;
class Renderer;
class SpriteManager;

enum Direction
{
	Up,
	Down,
	Left,
	Right,
	None
};

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

	int mCol = 0;
	int mRow = 0;

	bool operator==(const Tile& aOtherTile)
	{ 
		return mCol == aOtherTile.mCol && mRow == aOtherTile.mRow;
	}

	bool operator!=(const Tile& aOtherTile) { return !(mCol == aOtherTile.mCol && mRow == aOtherTile.mRow); }
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
	void RemovePickUp(int aCol, int aRow);
	inline int GetPickupCount() const { return mPickupCount; }

	std::vector<Direction> ComputePath(
		int aStartCol, int aStartRow, int aDestCol, int aDestRow, Direction aDirectionFrom) const;
	inline float GetManhattanDistance(int aStartCol, int aStartRow, int aDestCol, int aDestRow) const;
	Direction GetDirectionToMove(int aFromCol, int aFromRow, int aToCol, int aToRow) const;
	Direction GetOppositeDirection(Direction aDirection) const;

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