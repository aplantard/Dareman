#pragma once

#include <chrono>
#include <GameEngine/Level.h>

class Ghost : public GameActor
{
public:
	Ghost(Character aCharacter, int aPosX = 0, int aPosY = 0);
	~Ghost();

	void Render(Renderer* aRenderer) const;
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;
	int GetNumberOfPathAvailableFromPos(int aCol, int aRow, Direction aDirectionFrom);

private:
	Character mCharacter;
	std::vector<Direction> mDirections;
	SpriteSheet* mSpriteSheet;
};