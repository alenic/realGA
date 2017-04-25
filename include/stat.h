#ifndef STAT_H
#define STAT_H
#include <stdlib.h>
#include <math.h>

class Stat {
public:
	Stat();
	float uniformRand();
	float uniformRand(float lb, float ub);
	float gaussianRand(float m, float s);
	int uniformIndex(int N);
};

#endif // STAT_H