#include "SpriteManager.h"

#include <cassert>

#include "Dareman.h"
#include "Renderer.h"
#include "Sprite.h"

SpriteManager* SpriteManager::sInstance = nullptr;

void SpriteManager::CreateInstance(Renderer* aRenderer)
{
	assert(sInstance == nullptr);
	sInstance = new SpriteManager(aRenderer);
}

void SpriteManager::DeleteInstance()
{
	delete sInstance;
	sInstance = nullptr;
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
		Sprite* sprite = mRenderer->LoadSprite(aPath);
		sprite->SetSize(TILE_SIZE, TILE_SIZE);
		mSprites[aPath] = sprite;
		return sprite;
	}
}

SpriteManager::SpriteManager(Renderer* aRenderer)
	: mRenderer(aRenderer)
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
