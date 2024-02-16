#include "SpriteManager.h"

#include <cassert>
#include <SDL_image.h>

#include "Dareman.h"
#include "Renderer.h"
#include "Sprite.h"

SpriteManager* SpriteManager::sInstance = nullptr;

void SpriteManager::CreateInstance()
{
	assert(sInstance == nullptr);
	sInstance = new SpriteManager();
}

void SpriteManager::DeleteInstance()
{
	delete sInstance;
	sInstance = nullptr;
}

Sprite* SpriteManager::LoadSprite(const char* aPath)
{
	Renderer* renderer = Renderer::GetInstance();
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
	Renderer* renderer = Renderer::GetInstance();
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

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
	// Free all the sprites
	for (auto& it : mSprites)
	{
		delete it.second;
	}

	mSprites.clear();
}
