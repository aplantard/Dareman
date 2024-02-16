#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <thread>

#include "Dareman.h"
#include "Ghost.h"
#include "Level.h"
#include "Renderer.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "GameStateMgr.h"
#include "PlayerInputMgr.h"
#include "GameUI.h"
#include "GameEngine/GameEngine.h"

bool InitApp()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return false;

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return false;

	if (TTF_Init() != 0)
		return false;

	GameEngine* gameEngine = GameEngine::CreateInstance();

	if (gameEngine == nullptr)
		return false;

	return true;
}

void ShutdownApp()
{
	GameEngine::DeleteInstance();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!InitApp())
		return 1;

	GameEngine* gameEngine = GameEngine::GetInstance();
	gameEngine->LoadLevel("Data/Level/");

		if (!level->IsValid())
		{
			ShutdownApp();
			return 1;
		}
	

	GameStateMgr* gameStateMgr = gameEngine->GetGameStateMgr();
	PlayerInputMgr* playerInputMgr = gameEngine->GetPlayerInputMgr();
	Renderer* renderer = gameEngine->GetRenderer();

	if (gameStateMgr == nullptr || playerInputMgr == nullptr || renderer == nullptr)
	{
		ShutdownApp();
		return 1;
	}

	// Create game entities
	Dareman* dareman = new Dareman();
	const auto& daremanPosition = level->GetStartPosition(Character::Dareman);
	dareman->SetPosition(daremanPosition.first * TILE_SIZE, daremanPosition.second * TILE_SIZE);

	Ghost* ghosts[4];

	ghosts[0] = new Ghost(Character::Blinky);
	ghosts[1] = new Ghost(Character::Inky);
	ghosts[2] = new Ghost(Character::Pinky);
	ghosts[3] = new Ghost(Character::Clyde);

	// Startup dialog
	renderer->BeginFrame();
	renderer->SetOffset(0, headerHeight);
	level->Render(renderer);
	ShowDialog(renderer, pacmanFont, openSansFont, "dareman", "Press a key to start");


	// Main application loop
	auto now = std::chrono::high_resolution_clock::now();
	auto lastFrame = now;

	while (gameStateMgr->GetCurrentGameState() != GameStateMgr::GameState::Quit
		&& gameStateMgr->GetCurrentGameState() != GameStateMgr::GameState::Error)
	{
		now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> deltaTime = now - lastFrame;
		lastFrame = now;

		gameEngine->Update(deltaTime);

		// Limit FPS to 100, do not remove this as a means of optimization
		if (deltaTime.count() < 10)
		{
			std::chrono::duration<double, std::milli> sleepMs(10 - deltaTime.count());
			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(sleepMs));
			deltaTime = deltaTime + sleepMs;
		}

		GameUI* gameUI = GameEngine::GetInstance()->GetGameUI();
		renderer->SetOffset(0, gameUI->GetHeaderHeight());

		dareman->Render(renderer);
		for (int i = 0; i < 4; i++)
		{
			ghosts[i]->Render(renderer);
		}

		renderer->EndFrame();
	}


	bool isError = gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Error;

	delete dareman;
	dareman = nullptr;

	for (Ghost* currentGhost : ghosts)
	{
		delete currentGhost;
	}

	ShutdownApp();
	return isError ? 1 : 0;
}