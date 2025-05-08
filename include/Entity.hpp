#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector.hpp"


class Entity
{
protected:
	SDL_Texture* tex;
	SDL_Rect currentFrame;
	Vector loc; // location
	double angle; // angle of rotation
public:
	Entity();
	Entity(SDL_Texture* p_tex, Vector p_loc, double p_angle=0);
	Vector getLoc();
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
	double getAngle();
	~Entity();
};