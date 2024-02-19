#include <SDL_ttf.h>

#include <GameEngine/Renderer.h>
#include <GameEngine/Level.h>
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

	char fpsLabel[32];
	sprintf(fpsLabel, "%s %5.1f", mFpsLabel, (1000.f / mFpsCount));

	int fpsLabelWidth, fpsLabelHeight;
	TTF_SizeText(openSansFont, fpsLabel, &fpsLabelWidth, &fpsLabelHeight);

	const int fpsLabelX = level->GetWidthPx() - spacing - fpsLabelWidth;

	aRenderer->SetOffset(0, 0);
	aRenderer->DrawText(pacmanFont, mTitleLabel, YELLOW, spacing, spacing);
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