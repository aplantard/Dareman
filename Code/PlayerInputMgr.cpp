#include <cassert>
#include <chrono>
#include <SDL.h>

#include "GameStateMgr.h"
#include "PlayerInputMgr.h"

PlayerInputMgr::PlayerInputMgr() {}

PlayerInputMgr::~PlayerInputMgr() {}

void PlayerInputMgr::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	// Handle input
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				GameStateMgr::GetInstance()->SetCurrentState(GameStateMgr::GameState::Quit);
				break;
			}
			case SDLK_LEFT: playerInput = Left; break;
			case SDLK_RIGHT: playerInput = Right; break;
			case SDLK_UP: playerInput = Up; break;
			case SDLK_DOWN: playerInput = Down; break;
			default:
				// Ignore other keys
				break;
			}
		}
		else if (event.type == SDL_QUIT)
		{
			GameStateMgr::GetInstance()->SetCurrentState(GameStateMgr::GameState::Quit);
		}
	}
}

