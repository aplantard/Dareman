#include <cassert>
#include <chrono>
#include <PlayerInputMgr.h>
#include <Renderer.h>
#include <SpriteManager.h>
#include <GameStateMgr.h>
#include <Level.h>
#include <GameUI.h>
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

void GameEngine::LoadLevel(const char* aPath)
{
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
	
}

// Here order of creation is important
GameEngine::GameEngine()
{
	mRenderer = new Renderer();
	mGameStateMgr = new GameStateMgr(GameStateMgr::GameState::Lobby);
	mPlayerInputMgr = new PlayerInputMgr();
	mSpriteManager = new SpriteManager();
	mLevel = new Level();
	mGameUI = new GameUI();
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
}
