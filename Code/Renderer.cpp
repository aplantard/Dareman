#include <chrono>
#include <cassert>
#include <SDL_render.h>
#include <GameEngine/GameEngine.h>
#include <GameUI.h>
#include <Level.h>

#include "SpriteManager.h"
#include "Sprite.h"

#include "Renderer.h"

Renderer::Renderer()
	: mOffsetX(0)
	, mOffsetY(0)
{
	// The renderer is intentionally set to a software renderer.
	// Do not use hardware rendering as means of optimization in the context of the test.
	mWindow = SDL_CreateWindow("Dareman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_SOFTWARE);

	// Set clear color to black
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);

	mOpenSansFont = TTF_OpenFont("Data/Fonts/OpenSans-Regular.ttf", 16);
	mPacmanFont = TTF_OpenFont("Data/Fonts/Pac-Font.ttf", 20);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;

	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
}

SDL_Texture* Renderer::CreateTextureFromSurface(SDL_Surface* aSurface)
{
	return SDL_CreateTextureFromSurface(mRenderer, aSurface);
}

void Renderer::SetOffset(int anOffsetX, int anOffsetY)
{
	mOffsetX = anOffsetX;
	mOffsetY = anOffsetY;
}

void Renderer::SetWindowSize(int aSizeX, int aSizeY)
{
	// Adjust window to fit the level
	SDL_SetWindowSize(mWindow, aSizeX, aSizeY);
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Renderer::DrawText(TTF_Font* aFont, const char* aText, int aRGBAColor, int aX, int aY)
{
	SDL_Color textColor;
	textColor.r = (aRGBAColor >> 24) & 0xff;
	textColor.g = (aRGBAColor >> 16) & 0xff;
	textColor.b = (aRGBAColor >> 8) & 0xff;
	textColor.a = (aRGBAColor & 0xff);

	SDL_Surface* surface = TTF_RenderText_Shaded(aFont, aText, textColor, {0x00, 0x00, 0x00, 0x00});
	if (surface)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);

		SDL_Rect destRect;
		destRect.x = aX + mOffsetX;
		destRect.y = aY + mOffsetY;
		destRect.w = surface->w;
		destRect.h = surface->h;
		SDL_RenderCopy(mRenderer, texture, nullptr, &destRect);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}
}

void Renderer::DrawSprite(const Sprite* aSprite, int aX, int aY)
{
	SDL_Rect destRect;
	destRect.x = aX + mOffsetX;
	destRect.y = aY + mOffsetY;
	destRect.w = aSprite->mWidth;
	destRect.h = aSprite->mHeight;
	SDL_RenderCopy(mRenderer, aSprite->mTexture, nullptr, &destRect);
}

void Renderer::ShowDialog(TTF_Font* aTitleFont, TTF_Font* aTextFont, const char* aTitle, const char* aText)
{
	SpriteManager* spriteManager = SpriteManager::GetInstance();

	if (spriteManager)
	{
		const int spacing = 20;

		int titleWidth, titleHeight, textWidth, textHeight;
		TTF_SizeText(aTitleFont, aTitle, &titleWidth, &titleHeight);
		TTF_SizeText(aTitleFont, aTitle, &textWidth, &textHeight);

		const int dialogWidth = spacing * 2 + std::max(titleWidth, textWidth);
		const int dialogHeight = spacing * 3 + titleHeight + textHeight;

		Sprite* background = spriteManager->CreateRGBSprite(dialogWidth, dialogHeight, BLACK);

		int windowWidth, windowHeight;
		SDL_GetWindowSize(mWindow, &windowWidth, &windowHeight);

		const int dialogX = windowWidth / 2 - dialogWidth / 2;
		const int dialogY = windowHeight / 2 - dialogHeight / 2;

		DrawSprite(background, dialogX, dialogY);
		DrawText(aTitleFont, aTitle, YELLOW, dialogX + dialogWidth / 2 - titleWidth / 2, dialogY + spacing);
		DrawText(aTextFont, aText, YELLOW, dialogX + dialogWidth / 2 - textWidth / 2, dialogY + spacing * 2 + titleHeight);
		EndFrame();
	}
}

void Renderer::BeginFrame()
{
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	SDL_RenderPresent(mRenderer);
}

void Renderer::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	GameEngine* gameEngine = GameEngine::GetInstance();
	GameUI* gameUI = gameEngine->GetGameUI();

	BeginFrame();

	SetOffset(0, gameUI->GetHeaderHeight());
	gameEngine->GetLevel()->Render(this);

	//Render entities

	gameEngine->GetGameUI()->Render(this);


	//EndFrame();

}
