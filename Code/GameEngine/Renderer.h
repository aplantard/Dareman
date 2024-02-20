#pragma once

#include <SDL_ttf.h>
#include <queue>
#include <chrono>

#include <UI/Modal.h>

struct SDL_Renderer;
struct SDL_Window;
class Sprite;
class SpriteSheet;

#define BLACK 0x000000FF
#define YELLOW 0xFFFF00FF

class Renderer
{
public:

	Renderer();
	~Renderer();
	Renderer(const Renderer& anOther) = delete;

	inline SDL_Window* GetWindow() { return mWindow; }
	inline TTF_Font* GetOpenSansFont() { return mOpenSansFont; };
	inline TTF_Font* GetPacmanFont() { return mPacmanFont; };
	SDL_Texture* CreateTextureFromSurface(SDL_Surface* aSurface);

	// Adds an offset to all subsequent draw calls
	void SetOffset(int anOffsetX, int anOffsetY);

	void SetWindowSize(int aSizeX, int aSizeY);

	void DrawText(TTF_Font* aFont, const char* aText, int aRGBAColor, int aX, int aY);
	void DrawSprite(const Sprite* aSprite, int aX, int aY);
	void DrawSpriteFromSpriteSheet(const SpriteSheet* aSpriteSheet, int aX, int aY);

	void CreateDialog(const char* aTitle, const char* aText);
	void CloseDialog();
	inline bool ShouldPresentModal() { return mModals.size() > 0; };

	void BeginFrame();
	void EndFrame();

	void Update(std::chrono::duration<double, std::milli> aDeltaTime);

private:

	std::queue<Modal*> mModals;

	SDL_Renderer* mRenderer = nullptr;
	SDL_Window* mWindow = nullptr;

	TTF_Font* mOpenSansFont = nullptr;
	TTF_Font* mPacmanFont = nullptr;

	int mOffsetX = 0;
	int mOffsetY = 0;
};