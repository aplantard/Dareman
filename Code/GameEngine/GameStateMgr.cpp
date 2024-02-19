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
		case GameStateMgr::Win:
		{
			Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
			renderer->CreateDialog("you win !", "Congratulations");
			break;
		}
		case GameStateMgr::Loss:
		{
			Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
			renderer->CreateDialog("you lose!", "Try again");
			break;
		}
		default: break;
	}
}

