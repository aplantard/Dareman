#include <SDL_image.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>

#include "SpriteManager.h"

SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager()
{
	// Free all the sprites
	for (auto& it : mSprites)
	{
		delete it.second;
	}

	mSprites.clear();
}

Sprite* SpriteManager::LoadSprite(const char* aPath)
{
	Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
	if (renderer)
	{
		SDL_Surface* surface = IMG_Load(aPath);
		if (surface)
		{
			Sprite* sprite = new Sprite(renderer->CreateTextureFromSurface(surface), surface->w, surface->h);
			SDL_FreeSurface(surface);
			return sprite;
		}
		else
			return nullptr;
	}

	return nullptr;
}

Sprite* SpriteManager::GetSprite(const char* aPath)
{
	auto it = mSprites.find(aPath);
	if (it != mSprites.end())
	{
		return it->second;
	}
	else
	{
		Sprite* sprite = LoadSprite(aPath);
		sprite->SetSize(TILE_SIZE, TILE_SIZE);
		mSprites[aPath] = sprite;
		return sprite;
	}
}



Sprite* SpriteManager::CreateRGBSprite(int aWidth, int aHeight, int aRGBAColor)
{
	Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
	if (renderer)
	{
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, aWidth, aHeight, 32, SDL_PIXELFORMAT_ABGR32);
		if (surface)
		{
			SDL_FillRect(surface, nullptr, aRGBAColor);
			Sprite* sprite = new Sprite(renderer->CreateTextureFromSurface(surface), surface->w, surface->h);
			SDL_FreeSurface(surface);
			return sprite;
		}
		else
			return nullptr;
	}

	return nullptr;
}
