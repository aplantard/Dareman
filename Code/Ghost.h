#pragma once

#include <chrono>

class Ghost : public GameActor
{
public:
	Ghost(Character aCharacter, int aPosX = 0, int aPosY = 0);

	void Render(Renderer* aRenderer) const;
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;

private:
	Character mCharacter;
	Direction mDirection;
};