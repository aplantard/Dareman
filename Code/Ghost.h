#pragma once
#include <GameActor.h>

class SpriteSheet;

enum GhostState
{
	Chasing,
	Fleeing,
	Scatter,
	Recovering,
};

class Ghost : public GameActor
{
public:
	Ghost(Character aCharacter, int aPosX = 0, int aPosY = 0);
	~Ghost();

	int GetNumberOfPathAvailableFromPos(int aCol, int aRow, Direction aDirectionFrom);
	void UpdateSprite() const;
	void ToggleSprite() const;
	inline GhostState GetState() const { return mState; };
	inline void SetState(GhostState aNewGhostState) { mState = aNewGhostState;};
	inline void SetTurnBackFlee(bool aTurnBackAllowed) { mAllowTurnBackWhenFlee = aTurnBackAllowed;};
	void ClearDirections();
	Direction ComputeFleeingDirection();
	Tile ComputeChasingTargetTile();
	void UpdateState(std::chrono::duration<double, std::milli> aDeltaTime);
	bool IsAllowedToLeaveGhostHouse();

	virtual void Render(Renderer* aRenderer) const override;
	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;
	virtual bool CanMove(Direction aDirection) const override;
	virtual bool IsDareman() const override { return false; };

private:
	std::vector<Direction> mDirections;
	SpriteSheet* mSpriteSheet;
	Tile mDefaultTargetTile[3];
	Tile mTartgetTile;
	Tile mPreviousTile;
	GhostState mState = GhostState::Chasing;
	const float mScatterTotalDuration = 7000;
	const float mChasingTotalDuration = 20000;
	float mChangeStateDuration = 0;
	float mDistanceMoved = 0;
	int mScatterCount = 0;
	int mDefaultTargetTileIndex = 0;
	const int mNbScatterMax = 4;
	bool mAllowTurnBackWhenFlee = false;
	Character mCharacter;	
};