#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <random>
#include "Random.hpp"
#include "Map.hpp"


class PSO
{
private:
	const double W; // coefficient of inertia
	const double G; // social factor
	const double L; // cognitive factor

	Map* map;

	// double bestGlobalItn; // max intensity detected by all robots
	// Vector bestGlobalLoc; // location of global max intensity

	std::uniform_real_distribution<double> Gdis; // social factor distribution
	std::uniform_real_distribution<double> Ldis; // cognitive factor distribution

public:

	PSO(double W, double G, double L, Map* map);

	void communicateBestIntensityLoc();

	void updateVelBasedOnGradient();

	void updateVelocities();
};