#pragma once
#include <GameActor.h>

class SpriteSheet;

enum GhostState
{
	Chasing,
	Fleeing,
	Recovering,
};

class Ghost : public GameActor
{
public:
	Ghost(Character aCharacter, int aPosX = 0, int aPosY = 0);
	~Ghost();

	int GetNumberOfPathAvailableFromPos(int aCol, int aRow, Direction aDirectionFrom);
	void UpdateSprite();
	void ToggleSprite();
	inline GhostState GetState() { return mState; };
	inline void SetState(GhostState aNewGhostState) { mState = aNewGhostState;};
	void ClearDirections();

	virtual void Render(Renderer* aRenderer) const override;
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;
	virtual bool CanMove(Direction aDirection) const override;
	virtual bool IsDareman() const override { return false; };

private:
	Character mCharacter;
	GhostState mState = GhostState::Chasing;
	std::vector<Direction> mDirections;
	SpriteSheet* mSpriteSheet;

	float mDistanceMoved = 0;
};