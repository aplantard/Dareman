#include <cassert>
#include <GameEngine/GameEngine.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/Renderer.h>
 
#include "Ghost.h"



Ghost::Ghost(Character aCharacter, int aPosX, int aPosY)
	: mCharacter(aCharacter)
	, mDirection(Up)
	, GameActor(aPosX,aPosY)
{
	const char* spritePath = nullptr;
	switch (aCharacter)
	{
	case Character::Blinky: spritePath = "Data/Images/blinky.png"; break;
	case Character::Inky: spritePath = "Data/Images/inky.png"; break;
	case Character::Pinky: spritePath = "Data/Images/pinky.png"; break;
	case Character::Clyde: spritePath = "Data/Images/clyde.png"; break;
	default: assert(false && "This character is not a ghost");
	}

	mSprite = GameEngine::GetInstance()->GetSpriteManager()->GetSprite(spritePath);
}

void Ghost::Render(Renderer* aRenderer) const
{
	aRenderer->DrawSprite(mSprite, (int)mPosX, (int)mPosY);
}

void Ghost::Update(std::chrono::duration<double, std::milli> aDeltaTime) 
{
}
