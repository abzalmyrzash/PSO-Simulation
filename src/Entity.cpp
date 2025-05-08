#include "Entity.hpp"
#include <iostream>

Entity::Entity() {
	angle = 0;
}

Entity::Entity(SDL_Texture* p_tex, Vector p_loc, double p_angle)
	: tex(p_tex), loc(p_loc), angle(p_angle)
{
	SDL_QueryTexture(tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
	currentFrame.x = 0;
	currentFrame.y = 0;
}

Vector Entity::getLoc()
{
	return loc;
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

double Entity::getAngle()
{
	return angle;
}

Entity::~Entity()
{
	// if(tex != NULL) delete tex;
}