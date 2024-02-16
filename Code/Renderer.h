#pragma once

#include <SDL_ttf.h>

struct SDL_Renderer;
struct SDL_Window;
class Sprite;

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

	void ShowDialog(TTF_Font* aTitleFont, TTF_Font* aTextFont, const char* aTitle, const char* aText);

	void BeginFrame();
	void EndFrame();

	void Update(std::chrono::duration<double, std::milli> aDeltaTime);

private:

	SDL_Renderer* mRenderer = nullptr;
	SDL_Window* mWindow = nullptr;

	TTF_Font* mOpenSansFont = nullptr;
	TTF_Font* mPacmanFont = nullptr;

	int mOffsetX = 0;
	int mOffsetY = 0;
};