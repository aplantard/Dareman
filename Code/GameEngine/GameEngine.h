#pragma once
#include <chrono>
#include <vector>

class PlayerInputMgr;
class Renderer;
class GameStateMgr;
class Level;
class SpriteManager;
class GameUI;
class GameActor;
class Dareman;

class GameEngine
{
public:
	static GameEngine* CreateInstance();
	static void DeleteInstance();

	static inline GameEngine* GetInstance() { return sInstance; }

	inline Renderer* GetRenderer() { return mRenderer; };
	inline GameStateMgr* GetGameStateMgr() { return mGameStateMgr; };
	inline Level* GetLevel() { return mLevel; };
	inline SpriteManager* GetSpriteManager() { return mSpriteManager; };
	inline PlayerInputMgr* GetPlayerInputMgr() { return mPlayerInputMgr; };
	inline GameUI* GetGameUI() { return mGameUI; };
	inline std::vector<GameActor*> GetActors() const { return mGameActors; };
	Dareman* GetDareman();
	
	void LoadLevel(const char* aPath);
	void Update(std::chrono::duration<double, std::milli> aDeltaTime);
	void AddActor(GameActor* aActorToAdd);

private:
	GameEngine();
	~GameEngine();
	GameEngine(const GameEngine& anOther) = delete;

	Renderer* mRenderer;
	GameStateMgr* mGameStateMgr;
	Level* mLevel;
	PlayerInputMgr* mPlayerInputMgr;
	SpriteManager* mSpriteManager;
	GameUI* mGameUI;

	std::vector<GameActor*> mGameActors;

	static GameEngine* sInstance;
};