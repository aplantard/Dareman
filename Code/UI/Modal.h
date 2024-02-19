#pragma once

class Renderer;
class Sprite;
class Modal
{
public:
	Modal(const char* aTitle, const char* aText);
	~Modal();

	void Render(Renderer* aRenderer) const;

private:
	Sprite* mBackground = nullptr;
	int mModalWidth = 0;
	int mModalHeight = 0;

	int mTitleWidth = 0;
	int mTitleHeight = 0;

	int mTextWidth = 0;
	int mTextHeight = 0;

	char* mTitleLabel = "Title";
	char* mTextLabel= "Text";
};