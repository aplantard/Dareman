#include "Renderer.h"

#include <SDL_image.h>
#include <SDL_render.h>

#include "Sprite.h"

Renderer::Renderer(SDL_Window* aWindow)
	: mOffsetX(0)
	, mOffsetY(0)
{
	// The renderer is intentionally set to a software renderer.
	// Do not use hardware rendering as means of optimization in the context of the test.
	mRenderer = SDL_CreateRenderer(aWindow, -1, SDL_RENDERER_SOFTWARE);

	// Set clear color to black
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;
}

Sprite* Renderer::LoadSprite(const char* aPath)
{
	SDL_Surface* surface = IMG_Load(aPath);
	if (surface)
	{
		Sprite* sprite = new Sprite(SDL_CreateTextureFromSurface(mRenderer, surface), surface->w, surface->h);
		SDL_FreeSurface(surface);
		return sprite;
	}
	else
		return nullptr;
}

Sprite* Renderer::CreateRGBSprite(int aWidth, int aHeight, int aRGBAColor)
{
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, aWidth, aHeight, 32, SDL_PIXELFORMAT_ABGR32);
	if (surface)
	{
		SDL_FillRect(surface, nullptr, aRGBAColor);
		Sprite* sprite = new Sprite(SDL_CreateTextureFromSurface(mRenderer, surface), surface->w, surface->h);
		SDL_FreeSurface(surface);
		return sprite;
	}
	else
		return nullptr;
}

void Renderer::SetOffset(int anOffsetX, int anOffsetY)
{
	mOffsetX = anOffsetX;
	mOffsetY = anOffsetY;
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

void Renderer::BeginFrame()
{
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	SDL_RenderPresent(mRenderer);
}
