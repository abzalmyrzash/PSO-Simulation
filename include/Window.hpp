#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.hpp"
#include "Label.hpp"
#include "Utils.hpp"

class Window
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* robotTex;
	SDL_Texture* sourceTex;
	int winW, winH; // window size
	SDL_Rect mapBox;
	Label timeLabel;
	Label numRobotsLabel;
	Label numSourcesLabel;
	

	const int bgr_R = 200, bgr_G = 200, bgr_B = 200, bgr_A = 255; // background color
	const int map_R = 255, map_G = 255, map_B = 255, map_A = 255; // map color
	const int mapOutl_R = 0, mapOutl_G = 0, mapOutl_B = 0, mapOutl_A = 255; // map outline color

	const double RESIZE_COEFF = 0.5;
public:
	// wPer, hPer - width and height percentage relative to screen size
	Window(const char* p_title, float p_wPer, float p_hPer);

	SDL_Texture* loadTexture(const char* p_filePath);
	void loadTextures();
	SDL_Texture* getSourceTex();
	SDL_Texture* getRobotTex();

	int getDisplayIndex();

	int getRefreshRate();

	void getResolution(int* w, int* h);

	void getDPI(float* ddpi, float* hdpi, float* vdpi);

	void cleanUp();

	void clear();

	Vector rescaleLoc(Vector loc);

	void render(Entity* p_entity);

	void renderMap(Map* map);
	// void render(std::vector<Entity*>& p_entities);
	
	void display();

	void updateTimeLabel(double seconds, bool precise = false);

	void renderTimeLabel();

	void renderBestIntensityLoc(Robot *r);

	void renderCommRange(Robot *r);
};