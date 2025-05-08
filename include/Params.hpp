class Params
{
public:
	int numRobots = 100, numSources = 100;
	bool psoEnabled = true, gradEnabled = false;
	double W = 1.0, G=1.0, L=1.0;
	double robotMinSpeed = 1;
	double robotMaxSpeed = 100;
	double robotCommRange = 20;
	double sourceDetectionRadius = 1;
	double timeStep = 10;
	bool renderBestIntensityLoc = true, renderCommRange = false;
	bool neutralizeSources = true;
};