#ifndef TESTCOMMON_H
#define TESTCOMMON_H
#include "realgen.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstring>

using namespace std;

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
	RealGenotype *best;
};

void testRealGen(RealGen &ga, int maxIter, float eps, RealGenotype &expMin, GAResults &results);

void test_uniformDist();
void test_gaussianDist(int mean, int stDev);
void test_real_sphere(GAOptions opt, GAResults &results);
void test_rosenbrock(GAOptions opt, GAResults &results);


#endif