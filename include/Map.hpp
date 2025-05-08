#pragma once
#include "Robot.hpp"
#include "Sensor.hpp"
#include "Source.hpp"
#include <vector>
#include "Random.hpp"
#include "chrono"

class Map {
private:
	const double MAX_DISTANCE = sqrt(MAP_WIDTH * MAP_WIDTH + MAP_HEIGHT*MAP_HEIGHT);
	double SOURCE_DETECTION_RADIUS;

	std::uniform_real_distribution<double> xdis; // y distribution
	std::uniform_real_distribution<double> ydis; // x distribution
	std::uniform_real_distribution<double> itndis; // intensity distribution

	bool neutralizeSources;
	int numNeutralizedSources = 0;

	std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> finishTime;

	// 2D array of distances N robots have to M sources
	std::vector<std::vector<double> > distancesRS;
	// 2D array of distances N robots have between each other, with triangular form (N-1)x(N-1):
	// ***
	// **
	// *
	std::vector<std::vector<double> > distancesRR;

	double minDistance(Robot * r);
public:
	static constexpr double MAP_WIDTH = 150;
	static constexpr double MAP_HEIGHT = 100;

	std::vector<Robot*> robots;
	std::vector<Source*> sources;

	bool finished = false;

	Map(bool neutralizeSources);

	void init(int numRobots, int numSources,  SDL_Texture* sourceTex, SDL_Texture* robotTex,
			   double robotMinSpeed, double robotMaxSpeed, double robotCommRange, double SOURCE_DETECTION_RADIUS);

	void info();

	void start();

	double getElapsedTime();

	void calcDistances();

	// get distance between a robot and a source (r - robot index, s - source index)
	double getDistanceRS(int r, int s); 

	// get distance between two robots (r1, r2 - first and second robot indices)
	double getDistanceRR(int r1, int r2); 

	double sumMinDist();

	void updatePositions();

	void updateSources();

	void updateItn();

	~Map();
};