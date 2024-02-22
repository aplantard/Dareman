#include <chrono>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/Level.h>
#include <GameEngine/GameStateMgr.h>
#include <Dareman.h>

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
		case GameStateMgr::Run:
		{
			Level* level = GameEngine::GetInstance()->GetLevel();
			if (level->GetPickupCount() == 0)
			{
				mCurrentState = GameState::Win;
				Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
				renderer->CreateDialog("you win !", "Congratulations");
			}

			break;
		}
		case GameStateMgr::Win:
		{
			break;
		}
		case GameStateMgr::Loss:
		{
			break;
		}
		case GameStateMgr::DaremanDeath:
		{ 
			Dareman* dareman = GameEngine::GetInstance()->GetDareman();
			if (dareman->GetDaremanDeathAnimStep() >= 9)
			{
				mCurrentState = GameState::Loss;
				Renderer* renderer = GameEngine::GetInstance()->GetRenderer();
				renderer->CreateDialog("you lose!", "Try again");
			}
			dareman->DaremanDeathAnimation(aDeltaTime);
			break;
		}
		default: break;
	}
}

