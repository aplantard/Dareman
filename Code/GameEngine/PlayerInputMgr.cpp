#include <chrono>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Level.h>
#include "Dareman.h"
#include "GameStateMgr.h"
#include "Renderer.h"

#include "PlayerInputMgr.h"

PlayerInputMgr::PlayerInputMgr() {}

PlayerInputMgr::~PlayerInputMgr() {}

void PlayerInputMgr::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	GameEngine* gameEngine = GameEngine::GetInstance();
	GameStateMgr* gameStateMgr = gameEngine->GetGameStateMgr();
	Renderer* renderer = gameEngine->GetRenderer();

	// Handle input
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				gameStateMgr->SetCurrentState(GameStateMgr::GameState::Quit);
			}

			if (renderer->ShouldPresentModal() && gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Run)
			{
				renderer->CloseDialog();
			}
			else if (renderer->ShouldPresentModal() && (gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Win || gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Loss))
			{
				gameEngine->RestartGame();
				renderer->CloseDialog();
			}
			else if (gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Run)
			{
				Dareman* dareman = gameEngine->GetDareman();
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
				{
					dareman->SetWantedDirection(Left);
					break;
				}
				case SDLK_RIGHT:
				{
					dareman->SetWantedDirection(Right);
					break;
				}
				case SDLK_UP:
				{
					dareman->SetWantedDirection(Up);
					break;
				}
				case SDLK_DOWN:
				{
					dareman->SetWantedDirection(Down);
					break;
				}
				default:
					// Ignore other keys
					break;
				}
			}
		}
		else if (event.type == SDL_QUIT)
		{
			gameStateMgr->SetCurrentState(GameStateMgr::GameState::Quit);
		}
	}
}

