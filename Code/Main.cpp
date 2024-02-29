#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <thread>

#include "Dareman.h"
#include "Ghost.h"
#include "GameEngine/Level.h"
#include "GameEngine/Renderer.h"
#include "GameEngine/Sprite.h"
#include "GameEngine/SpriteManager.h"
#include "GameEngine/GameStateMgr.h"
#include "GameEngine/PlayerInputMgr.h"
#include "GameEngine/GameEngine.h"
#include "UI/GameUI.h"


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

	GameStateMgr* gameStateMgr = gameEngine->GetGameStateMgr();
	PlayerInputMgr* playerInputMgr = gameEngine->GetPlayerInputMgr();
	Renderer* renderer = gameEngine->GetRenderer();
	Level* level = gameEngine->GetLevel();

	if (!gameEngine->GetLevel()->IsValid())
	{
		ShutdownApp();
		return 1;
	}

	if (gameStateMgr == nullptr || playerInputMgr == nullptr || renderer == nullptr)
	{
		ShutdownApp();
		return 1;
	}

	// Display initial dialog
	renderer->CreateDialog("dareman", "Press a key to start");

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

		gameEngine->GetGameUI()->SetFpsCount(deltaTime.count() * 10);
	}

	bool isError = gameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Error;

	ShutdownApp();
	return isError ? 1 : 0;
}