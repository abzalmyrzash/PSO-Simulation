#include <SDL2/SDL.h>

class Slider
{
	SDL_Rect bar;
	double rangeMin;
	double rangeMax;
	double step;
	double value;
	bool scaleIsLogarithmic = false;
};