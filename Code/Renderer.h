#pragma once

#include <SDL_ttf.h>

struct SDL_Renderer;
struct SDL_Window;
class Sprite;

class Renderer
{
public:
	Renderer(SDL_Window* aWidow);
	~Renderer();

	Sprite* LoadSprite(const char* aPath);
	Sprite* CreateRGBSprite(int aWidth, int aHeight, int aRGBAColor);

	// Adds an offset to all subsequent draw calls
	void SetOffset(int anOffsetX, int anOffsetY);

	void DrawText(TTF_Font* aFont, const char* aText, int aRGBAColor, int aX, int aY);
	void DrawSprite(const Sprite* aSprite, int aX, int aY);

	void BeginFrame();
	void EndFrame();

private:
	SDL_Renderer* mRenderer;
	int mOffsetX;
	int mOffsetY;
};