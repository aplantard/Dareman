#pragma once

#define GAMEOBJECT_SPEED (TILE_SIZE * 6)

#include "GameActor.h"

enum Direction
{
	Up,
	Down,
	Left,
	Right,
	None
};

class Dareman : public GameActor
{
public:
	Dareman(int aPosX = 0, int aPosY = 0);
	~Dareman();

	void SetPosition(int aPosX, int aPosY);

	bool CanMove(Direction aDirection) const;
	bool IsOnTile() const;

	// Moves Dareman to the "next" tile.
	// If Dareman is not already on one tile, it finishes its current movement,
	// if it is on a tile it moves to the next tile.
	// Returns delta time remaining after the movement
	float MoveToNextTile(float aDeltaTime);
	void SetWantedDirection(Direction aNewDirection);
	void UpdateSprite();
	void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;

private:
	Direction mWantedDirection;
	Direction mDirection;
};