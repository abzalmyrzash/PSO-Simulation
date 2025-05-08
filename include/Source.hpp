#pragma once

#include <iostream>
#include "Vector.hpp"
#include "Entity.hpp"

class Source : public Entity
{
public:
	static constexpr double MAX_INTENSITY = 1000;
	static constexpr double MIN_INTENSITY = 100;

	using Entity::loc;
	double itn;  // intensity
	bool neutralized;

	Source();
	Source(SDL_Texture* p_tex, Vector p_loc, double p_itn);
	void info();
	void neutralize();
};