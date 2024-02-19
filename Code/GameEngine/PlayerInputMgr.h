#pragma once

class PlayerInputMgr
{
public:
	PlayerInputMgr();
	~PlayerInputMgr();
	PlayerInputMgr(const PlayerInputMgr& anOther) = delete;

	void Update(std::chrono::duration<double, std::milli> aDeltaTime);

private:


};