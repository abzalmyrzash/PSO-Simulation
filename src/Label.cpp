#include "Label.hpp"
// #include <windows.h>

Label::Label() {
    texture = nullptr;
}

Label::Label(SDL_Renderer *renderer, int x, int y)
{
    texture = nullptr;
    this->renderer = renderer;
    this->x = x;
    this->y = y;
}

void Label::loadFont(const char * filePath, int ptSize, SDL_Color textColor)
{
    font = TTF_OpenFont(filePath, ptSize);
    if (font == NULL){
        std::cout << "FONT NULL!!!" << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }
    this->textColor = textColor;
}

void Label::updateText(std::string& p_text) {
    if(text == p_text) return;
    text = p_text;

    if(texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    int text_width;
    int text_height;
    SDL_Surface *surface;

    surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = text_width;
    dstRect.h = text_height;

    SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
    srcRect.x = 0;
    srcRect.y = 0;
}

SDL_Texture* Label::getTexture() {
    return texture;
}

SDL_Rect Label::getSrcRect() {
    return srcRect;
}

SDL_Rect Label::getDstRect() {
    return dstRect;
}