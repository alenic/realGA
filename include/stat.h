/*

stat.h : genetic algorithm statistics

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

*/
#ifndef STAT_H_
#define STAT_H_
#include <stdlib.h>
#include <math.h>

class Stat {
public:
	Stat();
	float uniformRand();
	float uniformRand(float lb, float ub);
	float gaussianRand(float m, float s);
	int uniformIndex(int N);

	void setSeed(unsigned int seed);
};

#endif // STAT_H_