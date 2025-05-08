#include "Map.hpp"

Map::Map(bool neutralizeSources)
: neutralizeSources{neutralizeSources}{
	numNeutralizedSources = 0;

	xdis = std::uniform_real_distribution<double>(0, MAP_WIDTH);
	ydis = std::uniform_real_distribution<double>(0, MAP_HEIGHT);
	itndis = std::uniform_real_distribution<double>(Source::MIN_INTENSITY, Source::MAX_INTENSITY);
}

void Map::init(int numRobots, int numSources, SDL_Texture* sourceTex, SDL_Texture* robotTex,
			   double robotMinSpeed, double robotMaxSpeed, double robotCommRange, double sourceDetectionRadius)
{
	SOURCE_DETECTION_RADIUS = sourceDetectionRadius;

	for (int i = 0; i < numSources; i++) {
		Vector loc(xdis(Random::gen), ydis(Random::gen));
		double itn = itndis(Random::gen);
		Source * src = new Source(sourceTex, loc, itn);
		sources.push_back(src);
		// entities.push_back(&src);
	}

	for (int i = 0; i < numRobots; i++) {
		Vector loc(xdis(Random::gen), ydis(Random::gen));
		Vector vel(0, 0);
		Robot * rbt = new Robot(robotTex, loc, vel, robotMinSpeed, robotMaxSpeed, robotCommRange);
		rbt->moveRandomly();
		Sensor* sensor = new Sensor(rbt, &sources);
		robots.push_back(rbt);

		distancesRS.push_back(std::vector<double>(numSources, MAX_DISTANCE));
		distancesRR.push_back(std::vector<double>(numRobots-i-1, MAX_DISTANCE));
	}
}


void Map::info()
{
	for (int i = 0; i < sources.size(); i++) {
		std::cout << "Source #" << i + 1 << std::endl;
		sources[i]->info();
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
	for (int i = 0; i < robots.size(); i++) {
		std::cout << "Robot #" << i + 1 << std::endl;
		robots[i]->info();
		std::cout << "Minimum distance to a source: " << minDistance(robots[i]) << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl << "Sum of minimum distances: " << sumMinDist() << std::endl;
}

void Map::start()
{
	startTime = std::chrono::steady_clock::now();
	lastUpdateTime = std::chrono::steady_clock::now();
}

double Map::getElapsedTime()
{
	if(finished) return std::chrono::duration<double>(finishTime - startTime).count();
	auto now = std::chrono::steady_clock::now();
	double dt = std::chrono::duration<double>(now - startTime).count();
	return dt;
}

void Map::calcDistances()
{
	for (int i = 0; i < robots.size(); i++) {
		for (int j = 0; j < sources.size(); j++) {
			distancesRS[i][j] = dist(robots[i]->loc, sources[j]->loc);
		}
	}

	for (int i = 0; i < robots.size(); i++) {
		for (int j = i + 1; j < robots.size(); j++) {
			double distance = dist(robots[i]->loc, robots[j]->loc);
			distancesRR[i][j-i-1] = distance;
		}
	}
}

// r - index of robot, s - index of source
double Map::getDistanceRS(int r, int s)
{
	return distancesRS[r][s];
}

// r1, r2 - indices of robots
double Map::getDistanceRR(int r1, int r2)
{
	int i = std::min(r1, r2);
	int j = std::max(r1, r2);
	return distancesRR[i][j-i-1];
}

double Map::minDistance(Robot* r)
{
	double minD = MAX_DISTANCE;
	for (auto * src : sources) {
		minD = std::min(dist(r->loc, src->loc), minD);
	}
	return minD;
}

double Map::sumMinDist()
{
	double sum = 0;
	for (auto * r : robots) {
		sum += minDistance(r);
	}
	return sum;
}

void Map::updatePositions()
{
	auto now = std::chrono::steady_clock::now();
	double dt = std::chrono::duration<double>(now - lastUpdateTime).count();
	lastUpdateTime = now;
	for (auto * r : robots) {
		r->updateLoc(dt);
	}
}

void Map::updateSources()
{
	if(!neutralizeSources) return;
	if(finished) return;
	bool anyNeutralized = false;
	for (int i = 0; i < sources.size(); i++) {
		for (int j = 0; j < robots.size(); j++) {
			if (getDistanceRS(j, i) < SOURCE_DETECTION_RADIUS && !sources[i]->neutralized) {
				sources[i]->neutralize();
				numNeutralizedSources++;
				if(numNeutralizedSources == sources.size()) {
					finished = true;
					finishTime = std::chrono::steady_clock::now();

					for(auto* r : robots){
						r->stop();
					}
					return;
				}
				anyNeutralized = true;
				break;
			}
		}
	}
	if(anyNeutralized){
		for (auto * r : robots) {
			r->bestItn = 0;
			r->bestGlobalItn = 0;
		}
	}
}

void Map::updateItn()
{
	for (auto * r : robots) {
		r->sensor->updateIntensity();
	}
}

Map::~Map(){
	for (auto * r : robots) {
		delete r;
	}
	for (auto * s : sources) {
		delete s;
	}
}