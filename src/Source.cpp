#include "Source.hpp"

Source::Source()
{
	neutralized = false;
}

Source::Source(SDL_Texture* p_tex, Vector p_loc, double p_itn) : Entity(p_tex, p_loc)
{
	neutralized = false;
	itn = p_itn;
	currentFrame.w /= 2;
	currentFrame.x = 0;
	currentFrame.y = 0;
}

void Source::info()
{
	std::cout << "Location: " << "(" << loc.x << "; " << loc.y << ")\n";
	std::cout << "Intensity: ";
	if (!neutralized) std::cout << itn;
	else std::cout << "NEUTRALIZED";
	std::cout << std::endl;
}

void Source::neutralize()
{
	neutralized = true;
	itn = 0;
	currentFrame.x = currentFrame.w;
}