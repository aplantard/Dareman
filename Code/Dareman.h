#pragma once

#define GAMEOBJECT_SPEED (TILE_SIZE * 6)

#include "GameActor.h"

class SpriteSheet;

class Dareman : public GameActor
{
public:
	Dareman(int aCol = 0, int aRow = 0);
	~Dareman();
	
	bool CanMove(Direction aDirection) const;
	void SetWantedDirection(Direction aNewDirection);
	void UpdateSprite();
	void ToggleSprite();
	void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;
	void Render(Renderer* aRenderer) const override;

private:
	Direction mWantedDirection;
	SpriteSheet* mSpriteSheet;

	float mDistanceMoved = 0;
};