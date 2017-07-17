#ifndef REALGENOTYPE_H
#define REALGENOTYPE_H
#include "stat.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class RealGenotype {
public:
	vector<float> gene;
	double fitness;
	Stat stat;

	RealGenotype();
	RealGenotype(int n);
	RealGenotype(const RealGenotype &c);
	~RealGenotype();
	
	string toString();

	void uniformRandom();
	void uniformRandom(int);
	void uniformLocalRandom(int, float);
	void gaussianLocalRandom(int, float);
	void bound(float *, float *);
	double distanceTo(RealGenotype &);

	RealGenotype & operator= ( const RealGenotype &c );
	bool operator<(const RealGenotype &c) const;
	bool operator==(const RealGenotype &other) const;
};

#endif