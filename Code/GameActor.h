#pragma once
#include <chrono>

class Renderer;
class Sprite;

class GameActor
{
public:
	GameActor(int aPosX = 0, int aPosY = 0);
	virtual ~GameActor();

	void SetTilePosition(int aPosX, int aPosY);
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime){};
	virtual void Render(Renderer* aRenderer) const;

protected:
	Sprite* mSprite;
	float mPosX;
	float mPosY;
};