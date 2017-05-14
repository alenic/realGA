#ifndef TESTCOMMON_H
#define TESTCOMMON_H
#include "realgen.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;


void testRealGen(RealGen &ga, int maxIter, float eps, RealGenotype &expMin, bool &converged, int &iter, double &bestFitness, float &exTime);

void test_uniformDist();
void test_gaussianDist();
void test_real_sphere(GAOptions opt, bool &converged, int &iter, double &bestFitness, float &exTime);
void test_rosenbrock(GAOptions opt, bool &converged, int &iter, double &bestFitness, float &exTime);


#endif