#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class Label {
private:
	std::string text;

	int x, y;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Color textColor;

	SDL_Texture* texture;
	SDL_Rect srcRect, dstRect;

public:
	Label();
	Label(SDL_Renderer *renderer, int x, int y);

	void loadFont(const char * filePath, int ptSize, SDL_Color textColor);

	void updateText(std::string& text);

	SDL_Texture* getTexture();

	SDL_Rect getSrcRect();

	SDL_Rect getDstRect();
};