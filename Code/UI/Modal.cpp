#include <GameEngine/Renderer.h>
#include "GameEngine/SpriteManager.h"
#include "GameEngine/GameEngine.h"

#include "Modal.h"

const int spacing = 20;

Modal::Modal(const char* aTitle, const char* aText)
	: mTitleLabel((char* )aTitle)
	, mTextLabel((char*) aText)
{
	GameEngine* gameEngine = GameEngine::GetInstance();
	SpriteManager* spriteManager = gameEngine->GetSpriteManager();
	Renderer* renderer = gameEngine->GetRenderer();
	if (spriteManager)
	{
		TTF_Font* pacmanFont = renderer->GetPacmanFont();
		TTF_Font* openSansFont = renderer->GetOpenSansFont();

		TTF_SizeText(pacmanFont, mTitleLabel, &mTitleWidth, &mTitleHeight);
		TTF_SizeText(openSansFont, mTextLabel, &mTextWidth, &mTextHeight);

		mModalWidth = spacing * 2 + std::max(mTitleWidth, mTextWidth);
		mModalHeight = spacing * 3 + mTitleHeight + mTextHeight;

		mBackground = spriteManager->CreateRGBSprite(mModalWidth, mModalHeight, BLACK);
	}
}

Modal::~Modal() 
{
	delete mBackground;
	mBackground = nullptr;
}

void Modal::Render(Renderer* aRenderer) const
{
	SpriteManager* spriteManager = GameEngine::GetInstance()->GetSpriteManager();

	if (spriteManager)
	{
		TTF_Font* pacmanFont = aRenderer->GetPacmanFont();
		TTF_Font* openSansFont = aRenderer->GetOpenSansFont();

		int windowWidth, windowHeight;
		SDL_GetWindowSize(aRenderer->GetWindow(), &windowWidth, &windowHeight);

		const int dialogX = windowWidth / 2 - mModalWidth / 2;
		const int dialogY = windowHeight / 2 - mModalHeight / 2;

		aRenderer->DrawSprite(mBackground, dialogX, dialogY);
		aRenderer->DrawText(pacmanFont, mTitleLabel, YELLOW, dialogX + mModalWidth / 2 - mTitleWidth / 2, dialogY + spacing);
		aRenderer->DrawText(openSansFont, mTextLabel, YELLOW, dialogX + mModalWidth / 2 - mTextWidth / 2, dialogY + spacing * 2 + mTitleHeight);
	}
}
