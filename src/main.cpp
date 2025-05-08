#define SDL_MAIN_HANDLED
#include "PSO.hpp"
#include "Window.hpp"
#include "Params.hpp"
#include "Utils.hpp"
// #include <windows.h>
#include <fstream>
#include <thread>
#include <chrono>
typedef std::chrono::steady_clock Clock;
typedef std::chrono::duration<double, std::milli> duration_ms;

static void sleep_for(double dt)
{
    // static constexpr duration_ms MinSleepDuration(0);
    // Clock::time_point start = Clock::now();
    // while (duration_ms(Clock::now() - start).count() < dt) {
    //     std::this_thread::sleep_for(MinSleepDuration);
    // }
    std::this_thread::sleep_for(duration_ms(dt));
}

//Screen dimension constants
const int SCREEN_WIDTH_PERCENTAGE = 75;
const int SCREEN_HEIGHT_PERCENTAGE = 75;

Params params;
Window* window;
Map* map;
PSO* pso;
SDL_Event event;
bool running;

void init(){
	Random::gen = std::mt19937(static_cast<long unsigned int>(time(0)));

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "HEY... SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
	}
	if (!IMG_Init(IMG_INIT_PNG))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
	if (TTF_Init() == -1)
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;

	window = new Window("PSO Simulation", SCREEN_WIDTH_PERCENTAGE, SCREEN_HEIGHT_PERCENTAGE);
	window->loadTextures();

	map = new Map(params.neutralizeSources);
	map->init(params.numRobots, params.numSources, window->getSourceTex(), window->getRobotTex(),
			  params.robotMinSpeed, params.robotMaxSpeed, params.robotCommRange, params.sourceDetectionRadius);

	pso = new PSO(params.W, params.G, params.L, map);
	// map->info();

	running = true;

	window->clear();

	window->renderMap(map);
	window->updateTimeLabel(0);
	window->renderTimeLabel();

	window->display();
}

void eventHandlingTask(){
	while(running){
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_TEXTINPUT:
					std::cout << "SDL TEXT INPUT!" << std::endl;
					break;
				case SDL_TEXTEDITING:
					std::cout << "SDL TEXT EDITING!" << std::endl;
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_MOUSEBUTTONUP:
					break;
			}
		}
	}
}

void renderingTask(){
	double refreshTime = 1000.0/window->getRefreshRate();
	std::chrono::time_point<Clock> frameStartTime;

	while(running){
		auto oldFrameStartTime = frameStartTime;
		frameStartTime = Clock::now();
		double oldFrameTime = duration_ms(frameStartTime - oldFrameStartTime).count();

		window->clear();

		map->updatePositions();
		map->calcDistances();
		map->updateSources();
		window->updateTimeLabel(map->getElapsedTime(), true);
		// std::cout << map->getElapsedTime() << std::endl;
		window->renderMap(map);
		window->renderTimeLabel();

		if(params.renderBestIntensityLoc){
			for (auto * r : map->robots){
				window->renderBestIntensityLoc(r);
			}
		}
		if(params.renderCommRange){
			for (auto * r : map->robots){
				window->renderCommRange(r);
			}
		}

		window->display();

		double frameTime = duration_ms(Clock::now() - frameStartTime).count();

		if (frameTime < refreshTime) {
			sleep_for(refreshTime - frameTime);
		}
	}
}

void logicTask() {

	while (running) {
		auto stepStartTime = Clock::now();

		map->updatePositions();
		map->calcDistances();
		map->updateSources();

		if(map->finished) {
			break;
		}
		
		map->updateItn();
		if (params.psoEnabled) {
			pso->communicateBestIntensityLoc();
			pso->updateVelocities();
		}
		if (params.gradEnabled){
			pso->updateVelBasedOnGradient();
		}

		sleep_for(params.timeStep - duration_ms(Clock::now() - stepStartTime).count());
	}
}

int main (int argc, char* argv[])
{
    init();
    SDL_Delay(3000);
    map->start();

    double refreshTime = 1000.0/window->getRefreshRate();
	std::chrono::time_point<Clock> frameStartTime;

	while(running){
		auto oldFrameStartTime = frameStartTime;
		frameStartTime = Clock::now();
		double oldFrameTime = duration_ms(frameStartTime - oldFrameStartTime).count();

		map->updatePositions();
		map->calcDistances();
		map->updateSources();

		if(map->finished) {
			break;
		}
		
		map->updateItn();
		if (params.psoEnabled) {
			pso->communicateBestIntensityLoc();
			pso->updateVelocities();
		}
		if (params.gradEnabled){
			pso->updateVelBasedOnGradient();
		}

		window->clear();
		window->updateTimeLabel(map->getElapsedTime(), true);
		// std::cout << map->getElapsedTime() << std::endl;
		window->renderMap(map);
		window->renderTimeLabel();

		if(params.renderBestIntensityLoc){
			for (auto * r : map->robots){
				window->renderBestIntensityLoc(r);
			}
		}
		if(params.renderCommRange){
			for (auto * r : map->robots){
				window->renderCommRange(r);
			}
		}

		window->display();

		double frameTime = duration_ms(Clock::now() - frameStartTime).count();

		if (frameTime < refreshTime) {
			sleep_for(refreshTime - frameTime);
		}
	}



    // std::thread renderingThread(renderingTask);
    // std::thread logicThread(logicTask);

    // eventHandlingTask();
    // renderingThread.join();
    // logicThread.join();


	
	window->cleanUp();
	SDL_Quit();

	return 0;
}
