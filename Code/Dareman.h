#pragma once
#include <GameActor.h>

class SpriteSheet;

class Dareman : public GameActor
{
public:
	Dareman(int aCol = 0, int aRow = 0);
	~Dareman();
	
	void SetWantedDirection(Direction aNewDirection);
	void UpdateSprite();
	void ToggleSprite();
	void DaremanDeathAnimation(std::chrono::duration<double, std::milli> aDeltaTime);
	inline int GetDaremanDeathAnimStep() { return mDeathAnimationStepPassed; };

	virtual void Update(std::chrono::duration<double, std::milli> aDeltaTime) override;
	virtual void Render(Renderer* aRenderer) const override;
	virtual bool CanMove(Direction aDirection) const override;
	virtual bool IsDareman() const override { return true; };

private:
	Direction mWantedDirection;
	SpriteSheet* mSpriteSheet;

	float mDistanceMoved = 0;

	const float mDeathAnimationTotalDuration = 2000;
	int mDeathAnimationStepPassed = 0;
	float mDeathAnimationStepDuration = 0;
};