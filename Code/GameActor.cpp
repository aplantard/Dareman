#include <GameEngine/Renderer.h>
#include <GameEngine/Level.h>
#include "GameActor.h"

GameActor::GameActor(int aPosX, int aPosY)
	: mSprite(nullptr)
{
	SetTilePosition(aPosX, aPosY);
}

GameActor::~GameActor() {}

void GameActor::SetTilePosition(int aPosX, int aPosY)
{
	mPosX = float(aPosX * TILE_SIZE);
	mPosY = float(aPosY * TILE_SIZE);
}

void GameActor::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSprite(mSprite, (int)mPosX, (int)mPosY);
}
