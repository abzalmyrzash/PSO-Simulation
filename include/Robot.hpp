#pragma once

#include <iostream>
#include "Entity.hpp"
#include <cmath>
#include "Random.hpp"

#ifndef SENSOR_HPP
#define SENSOR_HPP

class Sensor;

class Robot : public Entity
{
private:
	std::uniform_real_distribution<double> spdis; // speed distribution
	std::uniform_real_distribution<double> dirdis; // direction distribution

	Vector vel;	// velocity

public:
	double maxSpeed;
	double minSpeed;
	double commRange; // range of communications

	Sensor* sensor;

	using Entity::loc; // location
	double itn; // detected intensity at the current location

	Vector oldLoc; // old location (from 1 iteration ago)
	double oldItn; // detected intensity at the old location

	double bestItn; // personal maximum intensity detected by the robot
	Vector bestLoc; // location of personal max intensity

	double bestGlobalItn; // global maximum intensity detected by other robots in communications range
	Vector bestGlobalLoc;

	Vector itnGrad; // intensity gradient
	Vector oldItnGrad;

	Robot (SDL_Texture* p_tex, Vector p_loc, Vector p_vel,
		   double p_minSpeed, double p_maxSpeed, double p_commRange);

	Vector getVel();
	
	void setVel(Vector p_vel);

	void stop();
	
	void updateAngle();

	void info();

	void updateLoc(double dt);

	void updateItn(double p_itn);

	void moveRandomly();

	void calcGradient();

	void updateVelBasedOnGradient();
};
#endif