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

	int mHeaderHeight = 0;
	float mFpsCount = 0.f;
	char mFpsLabel[15] = "FPS: ";
	char mTitleLabel[15] = "dareman";
	
};