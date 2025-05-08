#pragma once
#include "Robot.hpp"
#include "Source.hpp"
#include <vector>

class Sensor {
private:
	const double GRADIENT_STEP = 0.01;
	Robot* robot;
	std::vector<Source*>* sources;
	double intensity(Vector loc);
public:
	Sensor(Robot* robot, std::vector<Source*>* sources);
	double intensity();
	void updateIntensity();
	void calcGradient();
};