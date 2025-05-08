#include "Robot.hpp"

Vector Robot::getVel(){
	return vel;
}

void Robot::setVel(Vector p_vel) {
	vel = p_vel;
	double speed = vel.len();
	if (speed > maxSpeed) vel = vel * maxSpeed / speed;
	if (speed < minSpeed)
		moveRandomly();
	updateAngle();
}

void Robot::stop() {
	vel = Vector(0, 0);
}

void Robot::updateAngle() {
	angle = vel.angle();
}

void Robot::info(){
	std::cout << "Location: " << "(" << loc.x << "; " << loc.y << ")\n";
	std::cout << "Velocity: " << "(" << vel.x << "; " << vel.y << ")\n";
	std::cout << "Intensity at location: " << itn << std::endl;
}

Robot::Robot (SDL_Texture* p_tex, Vector p_loc, Vector p_vel,
			  double p_minSpeed, double p_maxSpeed, double p_commRange)
			  : Entity(p_tex, p_loc)
{
	minSpeed = p_minSpeed;
	maxSpeed = p_maxSpeed;
	commRange = p_commRange;
	spdis = std::uniform_real_distribution<double>(minSpeed, maxSpeed);
	dirdis = std::uniform_real_distribution<double>(0, 2 * M_PI);

	oldLoc = p_loc;
	bestLoc = p_loc;
	bestGlobalLoc = p_loc;

	itnGrad = Vector(0, 0);
	oldItnGrad = Vector(0, 0);

	vel = p_vel;
	angle = p_vel.angle();
}

// dt = delta time in seconds
void Robot::updateLoc(double dt) {
	oldLoc = loc;
	loc = loc + vel * dt;
}

void Robot::updateItn(double p_itn) {
	oldItn = itn;
	itn = p_itn;
	if (itn > bestItn) {
		bestItn = itn;
		bestLoc = loc;
	}
	if (itn > bestGlobalItn) {
		bestGlobalItn = itn;
		bestGlobalLoc = loc;
	}
}

void Robot::moveRandomly()
{
	double speed = spdis(Random::gen);
	double theta = dirdis(Random::gen);
	double vx = speed * cos(theta);
	double vy = speed * sin(theta);
	vel = Vector(vx, vy);
	updateAngle();
}

void Robot::updateVelBasedOnGradient() {
	if (itn < oldItn && itnGrad.len() > 0 && oldItnGrad.len() > 0) {
		double betaFR = itnGrad.len() / oldItnGrad.len();
		setVel(itnGrad * maxSpeed / itnGrad.len() + vel * betaFR);
	}
	
	oldLoc = loc;
	oldItn = itn;
	oldItnGrad = itnGrad;
}