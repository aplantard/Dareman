#pragma once
#include <chrono>

class Renderer;
class Sprite;
enum Direction;

class GameActor
{
public:
	GameActor(int aCol = 0, int aRow = 0);
	virtual ~GameActor();

	void SetTilePosition(int aCol, int aRow);
	inline std::pair<float, float> GetPosition() { return std::pair<float, float>(mPosX, mPosY); };

	bool IsOnTile() const;
	float MoveToNextTile(float aDeltaTime);

	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime){};
	virtual void Render(Renderer* aRenderer) const;

protected:
	Direction mDirection;
	Sprite* mSprite;
	float mPosX;
	float mPosY;
};