#pragma once

#include <map>
#include <string>
class Renderer;
class Sprite;

class SpriteManager
{
public:
	static void CreateInstance(Renderer* aRenderer);
	static void DeleteInstance();

	static SpriteManager& GetInstance() { return *sInstance; }

	Sprite* GetSprite(const char* aPath);

private:
	SpriteManager(Renderer* aRenderer);
	~SpriteManager();
	SpriteManager(const SpriteManager& anOther) = delete;

	std::map<std::string, Sprite*> mSprites;
	Renderer* mRenderer;

	static SpriteManager* sInstance;
};