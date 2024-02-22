#pragma once

class GameStateMgr
{
public:

	enum GameState
	{
		Run,
		Quit,
		Error,
		Win,
		Loss,
		DaremanDeath,
		None
	};

	GameStateMgr(GameState aInitialState);
	~GameStateMgr();
	GameStateMgr(const GameStateMgr& anOther) = delete;

	GameState GetCurrentGameState();
	inline void SetCurrentState(GameState aNewState) { mCurrentState = aNewState; };

	void Update(std::chrono::duration<double, std::milli> aDeltaTime);


private:

	GameState mCurrentState = GameState::None;

};