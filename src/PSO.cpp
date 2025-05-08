#include "PSO.hpp"
#include <ctime>


PSO::PSO(double W, double G, double L, Map* map) : W{W}, G{G}, L{L}, map{map}
{
	Gdis = std::uniform_real_distribution<double>(0, 1);
	Ldis = std::uniform_real_distribution<double>(0, 1);
}


void PSO::communicateBestIntensityLoc()
{
	for (int i = 0; i < map->robots.size(); i++) {
		Robot* robot_i = map->robots[i];
		for(int j = i + 1; j < map->robots.size(); j++) {
			Robot* robot_j = map->robots[j];
			// if two robots are within communications range, exchange personal best intensity info
			double dist = map->getDistanceRR(i, j);
			if (dist < robot_i->commRange && dist < robot_j->commRange) {
				if (robot_j->bestItn > robot_i->bestGlobalItn) {
					robot_i->bestGlobalItn = robot_j->bestItn;
					robot_i->bestGlobalLoc = robot_j->bestLoc;
				}
				if (robot_i->bestItn > robot_j->bestGlobalItn) {
					robot_j->bestGlobalItn = robot_i->bestItn;
					robot_j->bestGlobalLoc = robot_i->bestLoc;
				}
			}
		}
	}
}

void PSO::updateVelBasedOnGradient()
{
	for(auto * r : map->robots) {
		r->sensor->calcGradient();
		r->updateVelBasedOnGradient();
		r->updateAngle();
	}
}

void PSO::updateVelocities()
{
	for (auto * r : map->robots) {
		double Grand = Gdis(Random::gen);
		double Lrand = Ldis(Random::gen);
		Vector vel = r->getVel();
		vel = vel * W + (r->bestGlobalLoc - r->loc) * G * Grand + (r->bestLoc - r->loc) * L * Lrand;
		r->setVel(vel);
	}
}