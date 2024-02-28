#pragma once

class Renderer;

class GameUI
{
public:
	GameUI();

	void Render(Renderer* aRenderer) const;
	void CalculateHeaderHeight(Renderer* aRenderer);
	inline int GetHeaderHeight() { return mHeaderHeight; };
	inline void SetFpsCount(float aNewFpsValue) { mFpsCount = aNewFpsValue; };

private:
	char mFpsLabel[16] = "FPS: ";
	char mTitleLabel[16] = "dareman";
	char mScroreLabel[16] = "Score: ";
	float mFpsCount = 0.f;
	int mHeaderHeight = 0;	
};