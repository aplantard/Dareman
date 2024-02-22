#pragma once

#include <chrono>

class CollisionMgr
{
public:
	CollisionMgr();
	~CollisionMgr();


	void Update(std::chrono::duration<double, std::milli> aDeltaTime);

private:
};