#include <cassert>
#include <chrono>
#include <GameEngine/GameEngine.h>
#include "Renderer.h"
#include "GameStateMgr.h"

GameStateMgr::GameStateMgr(GameState aInitialState)
	: mCurrentState(aInitialState)
{
}

GameStateMgr::~GameStateMgr() {}


GameStateMgr::GameState GameStateMgr::GetCurrentGameState()
{
	return mCurrentState;
}

void GameStateMgr::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	switch (mCurrentState)
	{
		case GameStateMgr::Lobby:
		{
			Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
			renderer->ShowDialog(pacmanFont, openSansFont, "dareman", "Press a key to start");
			break;
		}
		case GameStateMgr::Run:
		{
			const float deltaSeconds = float(aDeltaTime.count()) / 1000.f;

			// Move dareman
			{
				if (dareman->mDirection == None)
					dareman->mDirection = playerInput;

				float remaining = deltaSeconds;
				while (remaining > 0.f && dareman->mDirection != None)
				{
					if (dareman->IsOnTile())
					{
						if (playerInput != None && dareman->CanMove(playerInput))
						{
							dareman->mDirection = playerInput;
							dareman->UpdateSprite();
						}

						if (dareman->CanMove(dareman->mDirection))
							remaining = dareman->MoveToNextTile(remaining);
						else
							dareman->mDirection = None; // Stop movement and exit loop
					}
					else
					{
						remaining = dareman->MoveToNextTile(remaining);
					}
				}
			}

			break;
		}
		case GameStateMgr::Win:
		{
			Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
			renderer->ShowDialog(pacmanFont, openSansFont, "you win !", "Congratulations");
			break;
		}
		case GameStateMgr::Loss:
		{
			Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
			renderer->ShowDialog(pacmanFont, openSansFont, "you lose!", "Try again");
			break;
		}
		default: break;
	}
}

