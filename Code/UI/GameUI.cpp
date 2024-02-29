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

	GameEngine* gameEngine = GameEngine::GetInstance();
	Level* level = gameEngine->GetLevel();

	char fpsLabel[32];
	sprintf(fpsLabel, "%s %5.1f", mFpsLabel, mFpsCount);

	int fpsLabelWidth, fpsLabelHeight;
	TTF_SizeText(openSansFont, fpsLabel, &fpsLabelWidth, &fpsLabelHeight);

	const int fpsLabelX = level->GetWidthPx() - spacing - fpsLabelWidth;

	char scoreLabel[32];
	sprintf(scoreLabel, "%s %u", mScroreLabel, gameEngine->GetPlayerScore());

	int scoreLabelWidth, scoreLabelHeight;
	TTF_SizeText(openSansFont, scoreLabel, &scoreLabelWidth, &scoreLabelHeight);

	const int scoreLabelX = (level->GetWidthPx() / 2) - (scoreLabelWidth / 2);

	aRenderer->SetOffset(0, 0);
	aRenderer->DrawText(pacmanFont, mTitleLabel, YELLOW, spacing, spacing);
	aRenderer->DrawText(openSansFont, fpsLabel, YELLOW, fpsLabelX, spacing);
	aRenderer->DrawText(openSansFont, scoreLabel, YELLOW, scoreLabelX, spacing);

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