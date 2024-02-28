#pragma once
#include <chrono>
#include <GameEngine/Level.h>

class Renderer;
class Sprite;
enum Direction;

constexpr float DEFAULT_SPEED = (TILE_SIZE * 6);

class GameActor
{
public:
	GameActor(int aCol = 0, int aRow = 0);
	virtual ~GameActor();

	void SetTilePosition(int aCol, int aRow);
	inline std::pair<float, float> GetPosition() { return std::pair<float, float>(mPosX, mPosY); };
	bool IsOnTile() const;
	float MoveToNextTile(float aDeltaTime);
	inline void SetSpeed(float aNewSpeed) { mSpeed = aNewSpeed; };

	virtual bool CanMove(Direction aDirection) const { return false; };
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime){};
	virtual void Render(Renderer* aRenderer) const;
	virtual bool IsDareman() const { return false; };

protected:
	Sprite* mSprite;
	Direction mDirection;
	float mPosX;
	float mPosY;
	float mSpeed = 0;
};