#include <GameEngine/GameEngine.h>
#include <GameActor.h>
#include <Ghost.h>
#include <Dareman.h>
#include "CollisionMgr.h"

CollisionMgr::CollisionMgr()
{
}

CollisionMgr::~CollisionMgr() {}

void CollisionMgr::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	GameEngine* gameEngine = GameEngine::GetInstance();
	Dareman* dareman = gameEngine->GetDareman();
	std::pair<float, float> daremanPos = dareman->GetPosition();

	for (auto gameActor : gameEngine->GetActors())
	{
		if (gameActor->IsDareman() == false)
		{
			Ghost* currentGhost = (Ghost*)gameActor;
			std::pair<float, float> currentGhostPos = currentGhost->GetPosition();
			GhostState ghostState = currentGhost->GetState();

			if ((int)daremanPos.first / TILE_SIZE == (int)currentGhostPos.first / TILE_SIZE && (int)daremanPos.second / TILE_SIZE == (int)currentGhostPos.second / TILE_SIZE)
			{
				if (ghostState == GhostState::Chasing)
				{
					gameEngine->KillDareman();
				}
				else if (ghostState == GhostState::Fleeing)
				{
					currentGhost->SetState(GhostState::Recovering);
					currentGhost->ClearDirections();
					currentGhost->SetSpeed(1.5f * DEFAULT_SPEED);
					gameEngine->EatGhost();
					
				}
			}
			else if (ghostState == GhostState::Recovering 
				&& (int)currentGhostPos.first / TILE_SIZE >= 11 
				&& (int)currentGhostPos.first / TILE_SIZE < 16
				&& (int)currentGhostPos.second / TILE_SIZE >= 13
				&& (int)currentGhostPos.second / TILE_SIZE < 15)
			{
				currentGhost->SetState(GhostState::Chasing);
				currentGhost->SetSpeed(DEFAULT_SPEED);
				currentGhost->ClearDirections();
			}
		}
	}
}
