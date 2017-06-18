#ifndef STAT_H
#define STAT_H
#include <stdlib.h>
#include <math.h>
#include <random>

class Stat {
public:
	Stat();
	float uniformRand();
	float uniformRand(float lb, float ub);
	float gaussianRand(float m, float s);
	int uniformIndex(int N);
private:
	//std::random_device rd;
	//std::ranlux24 gen;
};

#endif // STAT_H