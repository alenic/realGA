#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_
#include "realgen.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstring>

using namespace std;

enum TextColor {
	TEXT_GREEN = 0,
	TEXT_YELLOW,
	TEXT_RED
};

void coutColor(const char * s, TextColor color);

// Stat Unit Tests
class StatTest {
private:
	Stat stat;
	float uniformChiSquared(int *a, int n, int N);
public:
	StatTest();
	void test_uniformDist();
	void test_gaussianDist(int mean, int stDev);
};


// Realgenotype Unit Tests
class RealGenotypeTest {
public:
	RealGenotypeTest();
	void test_uniformRandom();
	void test_uniformLocalRandom();
	void test_gaussianLocalRandom();
	void test_bound();
	void test_distanceTo();
};



struct GAResults {
	char name[40];
	int Np;
	int maxIter;
	int iter;
	bool converged;
	float eps;
	double bestFitness;
	float maxTime;
	float exTime;
	RealGenotype best;
};

void testRealGen(RealGen &ga, int maxIter, float eps, RealGenotype &expMin, GAResults &results);
void test_real_sphere(GAOptions opt, GAResults &results);
void test_rosenbrock(GAOptions opt, GAResults &results);

#endif