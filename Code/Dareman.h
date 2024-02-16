#pragma once

#define TILE_SIZE 24
#define GAMEOBJECT_SPEED (TILE_SIZE * 6)

class Renderer;
class Sprite;

enum Direction
{
	Up,
	Down,
	Left,
	Right,
	None
};

class Dareman
{
public:
	Dareman();
	~Dareman();

	void SetPosition(int aPosX, int aPosY);

	bool CanMove(Direction aDirection) const;
	bool IsOnTile() const;

	// Moves Dareman to the "next" tile.
	// If Dareman is not already on one tile, it finishes its current movement,
	// if it is on a tile it moves to the next tile.
	// Returns delta time remaining after the movement
	float MoveToNextTile(float aDeltaTime);

	virtual void Render(Renderer* aRenderer) const;

	void UpdateSprite();

	Direction mDirection;
	Direction mWantedDirection;

private:
	Sprite* mCurrentSprite;
	float mPosX;
	float mPosY;
};