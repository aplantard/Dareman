#include <cassert>
#include <chrono>
#include <GameEngine/PlayerInputMgr.h>
#include <GameEngine/Renderer.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/GameStateMgr.h>
#include <GameEngine/Level.h>
#include <UI/GameUI.h>
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

void GameEngine::EatPickUp(int aCol, int aRow) 
{
	Tile currentTile = mLevel->GetTile(aCol, aRow);

	if (currentTile.mPickup == Pickup::Small)
	{
		mPlayerScore += 10;
		mLevel->RemovePickUp(aCol, aRow);
	}
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
	}
	
}

void GameEngine::AddActor(GameActor* aActorToAdd) 
{
	mGameActors.push_back(aActorToAdd);
}

// Here order of creation is important
GameEngine::GameEngine()
{
	mRenderer = new Renderer();
	mGameStateMgr = new GameStateMgr(GameStateMgr::GameState::Run);
	mPlayerInputMgr = new PlayerInputMgr();
	mSpriteManager = new SpriteManager();
	mLevel = new Level();
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

	for (auto gameActor : mGameActors)
	{
		delete gameActor;
	}

	mGameActors.clear();
}
