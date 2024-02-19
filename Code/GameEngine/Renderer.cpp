#include <GameEngine/GameEngine.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/Level.h>
#include <UI/GameUI.h>

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
	SpriteManager* spriteManager = GameEngine::GetInstance()->GetSpriteManager();

	SDL_Rect destRect;
	destRect.x = aX + mOffsetX;
	destRect.y = aY + mOffsetY;
	destRect.w = spriteManager->GetSpriteWidth(aSprite);
	destRect.h = spriteManager->GetSpriteHeight(aSprite);
	SDL_RenderCopy(mRenderer, spriteManager->GetSpriteTexture(aSprite), nullptr, &destRect);
}

void Renderer::CreateDialog(const char* aTitle, const char* aText)
{
	mModals.push(new Modal(aTitle, aText));
}

void Renderer::CloseDialog()
{
	Modal* modalToClose = mModals.front();
	delete modalToClose;

	mModals.pop();
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

	if (ShouldPresentModal())
	{
		mModals.front()->Render(this);
	}
	else
	{ 
		// Render entities
		for (auto gameActor : gameEngine->GetActors())
		{
			gameActor->Render(this);
		}

	}

	gameEngine->GetGameUI()->Render(this);

	EndFrame();
}
