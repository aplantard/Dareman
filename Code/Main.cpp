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

#define BLACK 0x000000FF
#define YELLOW 0xFFFF00FF

static SDL_Window* sWindow = nullptr;

enum GameState
{
	Run,
	Quit,
	Error,
	Win,
	Loss
};

bool InitApp()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return false;

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return false;

	if (TTF_Init() != 0)
		return false;

	sWindow = SDL_CreateWindow("Dareman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (sWindow == nullptr)
		return false;

	return true;
}

void ShutdownApp()
{
	if (sWindow)
		SDL_DestroyWindow(sWindow);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Shows a modal dialog that is hidden on keypress
// Returns false if ESC was pressed, true if any other key was pressed
bool ShowDialog(Renderer* aRenderer, TTF_Font* aTitleFont, TTF_Font* aTextFont, const char* aTitle, const char* aText)
{
	const int spacing = 20;

	int titleWidth, titleHeight, textWidth, textHeight;
	TTF_SizeText(aTitleFont, aTitle, &titleWidth, &titleHeight);
	TTF_SizeText(aTitleFont, aTitle, &textWidth, &textHeight);

	const int dialogWidth = spacing * 2 + std::max(titleWidth, textWidth);
	const int dialogHeight = spacing * 3 + titleHeight + textHeight;

	Sprite* background = aRenderer->CreateRGBSprite(dialogWidth, dialogHeight, BLACK);

	int windowWidth, windowHeight;
	SDL_GetWindowSize(sWindow, &windowWidth, &windowHeight);

	const int dialogX = windowWidth / 2 - dialogWidth / 2;
	const int dialogY = windowHeight / 2 - dialogHeight / 2;

	aRenderer->DrawSprite(background, dialogX, dialogY);
	aRenderer->DrawText(aTitleFont, aTitle, YELLOW, dialogX + dialogWidth / 2 - titleWidth / 2, dialogY + spacing);
	aRenderer->DrawText(aTextFont, aText, YELLOW, dialogX + dialogWidth / 2 - textWidth / 2, dialogY + spacing * 2 + titleHeight);
	aRenderer->EndFrame();

	// Modal event loop
	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE: return false;
				default: return true;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				return false;
			}
		}
	}

	return false;
}

int main(int argc, char* argv[])
{
	if (!InitApp())
		return 1;

	Renderer* renderer = new Renderer(sWindow);
	SpriteManager::CreateInstance(renderer);

	Level* level = new Level(renderer);
	level->LoadLevel("Data/Level/");

	if (!level->IsValid())
	{
		ShutdownApp();
		return 1;
	}

	TTF_Font* openSansFont = TTF_OpenFont("Data/Fonts/OpenSans-Regular.ttf", 16);
	TTF_Font* pacmanFont = TTF_OpenFont("Data/Fonts/Pac-Font.ttf", 20);

	int titleWidth, titleHeight, fpsLabelWidth, fpsLabelHeight;
	TTF_SizeText(pacmanFont, "dareman", &titleWidth, &titleHeight);
	TTF_SizeText(openSansFont, "FPS: 000.0", &fpsLabelWidth, &fpsLabelHeight);

	const int spacing = 4;
	const int headerHeight = std::max(titleHeight, fpsLabelHeight) + spacing * 2;
	const int fpsLabelX = level->GetWidthPx() - spacing - fpsLabelWidth;

	renderer->SetOffset(0, headerHeight);

	// Adjust window to fit the level
	SDL_SetWindowSize(sWindow, level->GetWidthPx(), level->GetHeightPx() + headerHeight);
	SDL_SetWindowPosition(sWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

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
	GameState gameState = Run;
	Direction playerInput = None;

	while (gameState == Run)
	{
		now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> deltaTime = now - lastFrame;
		lastFrame = now;

		// Handle input
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE: gameState = Quit; break;
				case SDLK_LEFT: playerInput = Left; break;
				case SDLK_RIGHT: playerInput = Right; break;
				case SDLK_UP: playerInput = Up; break;
				case SDLK_DOWN: playerInput = Down; break;
				default:
					// Ignore other keys
					break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					if (playerInput == Left)
						playerInput = None;
					break;
				case SDLK_RIGHT:
					if (playerInput == Right)
						playerInput = None;
					break;
				case SDLK_UP:
					if (playerInput == Up)
						playerInput = None;
					break;
				case SDLK_DOWN:
					if (playerInput == Down)
						playerInput = None;
					break;
				default:
					// Ignore other keys
					break;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				gameState = Quit;
			}
		}

		// Limit FPS to 100, do not remove this as a means of optimization
		if (deltaTime.count() < 10)
		{
			std::chrono::duration<double, std::milli> sleepMs(10 - deltaTime.count());
			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(sleepMs));
			deltaTime = deltaTime + sleepMs;
		}

		// Update game state
		if (gameState == Run)
		{
			const float deltaSeconds = float(deltaTime.count()) / 1000.f;

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

			// TODO: Move Ghosts, process pickups, win and lose conditions
		}

		// Render
		renderer->BeginFrame();
		renderer->SetOffset(0, headerHeight);

		level->Render(renderer);
		dareman->Render(renderer);
		for (int i = 0; i < 4; i++)
		{
			ghosts[i]->Render(renderer);
		}

		// Rendering Title and FPS
		renderer->SetOffset(0, 0);
		renderer->DrawText(pacmanFont, "dareman", YELLOW, spacing, spacing);

		char fpsLabel[15];
		sprintf(fpsLabel, "FPS: %5.1f", 1000.f / float(deltaTime.count()));
		renderer->DrawText(openSansFont, fpsLabel, YELLOW, fpsLabelX, spacing);

		renderer->EndFrame();
	}

	if (gameState == Win)
		ShowDialog(renderer, pacmanFont, openSansFont, "you win !", "Congratulations");
	else if (gameState == Loss)
		ShowDialog(renderer, pacmanFont, openSansFont, "you lose!", "Try again");

	SpriteManager::DeleteInstance();
	ShutdownApp();
	return gameState == Error ? 1 : 0;
}