#include <cassert>
#include <SDL_ttf.h>
#include <stdio.h>
#include <algorithm>
#include <Renderer.h>
#include <Level.h>
#include "GameEngine/GameEngine.h"
#include "GameUI.h"

const int spacing = 4;

GameUI::GameUI()
{
	
}

void GameUI::Render(Renderer* aRenderer) const
{
	TTF_Font* pacmanFont = aRenderer->GetPacmanFont();
	TTF_Font* openSansFont = aRenderer->GetOpenSansFont();

	Level* level = GameEngine::GetInstance()->GetLevel();
	int fpsLabelWidth, fpsLabelHeight;
	TTF_SizeText(openSansFont, mFpsLabel, &fpsLabelWidth, &fpsLabelHeight);

	const int fpsLabelX = level->GetWidthPx() - spacing - fpsLabelWidth;


	aRenderer->SetOffset(0, 0);
	aRenderer->DrawText(pacmanFont, mTitleLabel, YELLOW, spacing, spacing);

	char* fpsLabel = (char*)mFpsLabel;
	sprintf(fpsLabel + strlen(fpsLabel), "%5.1f", 1000.f / mFpsCount);
	aRenderer->DrawText(openSansFont, fpsLabel, YELLOW, fpsLabelX, spacing);

}

void GameUI::CalculateHeaderHeight(Renderer* aRenderer)
{
	TTF_Font* pacmanFont = aRenderer->GetPacmanFont();
	TTF_Font* openSansFont = aRenderer->GetOpenSansFont();

	int titleWidth, titleHeight, fpsLabelWidth, fpsLabelHeight;
	TTF_SizeText(pacmanFont, mTitleLabel, &titleWidth, &titleHeight);
	TTF_SizeText(openSansFont, mFpsLabel, &fpsLabelWidth, &fpsLabelHeight);

	mHeaderHeight = std::max(titleHeight, fpsLabelHeight) + spacing * 2;
}
