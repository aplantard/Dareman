#include <cassert>
#include <chrono>
#include <GameEngine/PlayerInputMgr.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/GameStateMgr.h>
#include <GameEngine/Level.h>
#include <GameEngine/CollisionMgr.h>
#include <UI/GameUI.h>
#include <Ghost.h>
#include <GameActor.h>
#include "GameEngine.h"

GameEngine* GameEngine::sInstance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
	assert(sInstance == nullptr);
	sInstance = new GameEngine();
	return sInstance;
}

void GameEngine::DeleteInstance()
{
	delete sInstance;
	sInstance = nullptr;
}

Dareman* GameEngine::GetDareman()
{
	Dareman* dareman = (Dareman*)mGameActors[mGameActors.size() - 1];
	return dareman;
}

void GameEngine::KillDareman()
{
	mGameStateMgr->SetCurrentState(GameStateMgr::GameState::DaremanDeath);
}

void GameEngine::EatPickUp(int aCol, int aRow)
{
	Tile currentTile = mLevel->GetTile(aCol, aRow);

	if (currentTile.mPickup == Pickup::Small)
	{
		mPlayerScore += 10;
		mLevel->RemovePickUp(aCol, aRow);
	}
	else if (currentTile.mPickup == Pickup::Big)
	{
		mPlayerScore += 50;
		mLevel->RemovePickUp(aCol, aRow);

		for (auto gameActor : mGameActors)
		{
			if (gameActor->IsDareman() == false)
			{	
				Ghost* currentGhost = (Ghost*)gameActor;
				
				if (currentGhost->GetState() != GhostState::Recovering)
				{
					currentGhost->SetState(GhostState::Fleeing);
					currentGhost->SetSpeed(0.5f * DEFAULT_SPEED);
					currentGhost->ToggleSprite();
					currentGhost->ClearDirections();
				}
			}
		}

		mEnergizerDuration = 0;
	}
}

void GameEngine::EatGhost()
{
	mNbGhostKilled++;
	mPlayerScore += 200 * mNbGhostKilled;
}

void GameEngine::LoadLevel(const char* aPath)
{
	mLevel->LoadTilesSprite();
	mLevel->LoadLevel(aPath);

	if (!mLevel->IsValid())
	{
		mGameStateMgr->SetCurrentState(GameStateMgr::GameState::Error);
		return;
	}

	mGameUI->CalculateHeaderHeight(mRenderer);
	mRenderer->SetWindowSize(mLevel->GetWidthPx(), mLevel->GetHeightPx() + mGameUI->GetHeaderHeight());
}

void GameEngine::Update(std::chrono::duration<double, std::milli> aDeltaTime)
{
	mPlayerInputMgr->Update(aDeltaTime);
	mGameStateMgr->Update(aDeltaTime);
	mRenderer->Update(aDeltaTime);

	if (mGameStateMgr->GetCurrentGameState() == GameStateMgr::GameState::Run && mRenderer->ShouldPresentModal() == false)
	{
		for (auto gameActor : mGameActors)
		{
			gameActor->Update(aDeltaTime);
		}
		
		mCollisionMgr->Update(aDeltaTime);

		// Manager Energizer
		{
			if (mEnergizerDuration >= 0 && mEnergizerDuration < mEnergizerTotalDuration)
			{
				mEnergizerDuration += aDeltaTime.count();
			}
			else if (mEnergizerDuration >= 0)
			{
				for (auto gameActor : mGameActors)
				{
					if (gameActor->IsDareman() == false)
					{
						Ghost* currentGhost = (Ghost*)gameActor;
						if (currentGhost->GetState() == GhostState::Fleeing)
						{
							currentGhost->SetState(GhostState::Chasing);
							currentGhost->SetSpeed(DEFAULT_SPEED);
						}
					}
				}
			}
		}
	}
}

void GameEngine::AddActor(GameActor* aActorToAdd) 
{
	mGameActors.push_back(aActorToAdd);
}

void GameEngine::RestartGame() 
{
	for (auto gameActor : mGameActors)
	{
		delete gameActor;
	}

	mGameActors.clear();
	mPlayerScore = 0;
	mNbGhostKilled = 0;
	mLevel->LoadLevel("Data/Level/");
	mGameStateMgr->SetCurrentState(GameStateMgr::GameState::Run);
}

// Here order of creation is important
GameEngine::GameEngine()
{
	mRenderer = new Renderer();
	mGameStateMgr = new GameStateMgr(GameStateMgr::GameState::Run);
	mPlayerInputMgr = new PlayerInputMgr();
	mSpriteManager = new SpriteManager();
	mLevel = new Level();
	mCollisionMgr = new CollisionMgr();
	mGameUI = new GameUI();
	mGameActors.reserve(16);
}

GameEngine::~GameEngine()
{
	delete mRenderer;
	mRenderer = nullptr;

	delete mGameStateMgr;
	mGameStateMgr = nullptr;

	delete mPlayerInputMgr;
	mPlayerInputMgr = nullptr;

	delete mSpriteManager;
	mSpriteManager = nullptr;

	delete mLevel;
	mLevel = nullptr;

	delete mGameUI;
	mGameUI = nullptr;

	delete mCollisionMgr;
	mCollisionMgr = nullptr;

	for (auto gameActor : mGameActors)
	{
		delete gameActor;
	}

	mGameActors.clear();
}
