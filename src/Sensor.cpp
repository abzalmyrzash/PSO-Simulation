#include "Sensor.hpp"

Sensor::Sensor(Robot* robot, std::vector<Source*>* sources) : robot{robot}, sources{sources} {
	robot->sensor = this;

	double itn = intensity();

	robot->itn = itn;
	robot->bestItn = itn;
	robot->bestGlobalItn = itn;
}

double Sensor::intensity(Vector loc){
	double itn = 0;
	for (auto * src : *sources) {
		if (!src->neutralized)
			itn += src->itn / pow((dist(loc, src->loc)) + 1, 2);
	}
	return itn;
}

double Sensor::intensity(){
	return intensity(robot->loc);
}

void Sensor::updateIntensity(){
	robot->updateItn(intensity());
}

void Sensor::calcGradient(){
	Vector loc_dx(robot->loc.x + GRADIENT_STEP, robot->loc.y);
	Vector loc_dy(robot->loc.x, robot->loc.y + GRADIENT_STEP);
	double dItn_x = intensity(loc_dx) - robot->itn;
	double dItn_y = intensity(loc_dy) - robot->itn;

	robot->itnGrad = Vector(dItn_x/GRADIENT_STEP, dItn_y/GRADIENT_STEP);
}