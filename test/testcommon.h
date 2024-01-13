#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_
#include "realgen.h"
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

enum TextColor {
    TEXT_GREEN = 0,
    TEXT_YELLOW,
    TEXT_RED
};

void coutColor(const char * s, TextColor color);

// Statistics Tests
class StatTest {
private:
    Stat stat;
    float uniformChiSquared(int *a, int n, int N);
public:
    StatTest();
    void test_uniformDist();
    void test_gaussianDist(int mean, int stDev);
};


// Realchromosome Tests
class RealChromosomeTest {
public:
    RealChromosomeTest();
    void test_randUniform();
    void test_uniformMutate();
    void test_gaussianMutate();
    void test_bound();
    void test_distanceTo();
};

// Selection Tests
class SelectionTests {
public:
    SelectionTests();
    void test_RWsearchIndexBinarySearch();
    void test_RWselect();
};



struct GAResults {
    string name;
    int Np;
    int maxIter;
    int iter;
    bool converged;
    float eps;
    float bestFitness;
    float maxTime;
    float convergedTime;
    RealChromosome best;
};

void printDistribution(vector<float> &x);

void testRealGen(RealGen &ga, int maxIter, float eps, GAResults &results);

// Benchmark
void benchmark(RealGenOptions opt, GAResults &results, int chromosomeSize, int populationSize);

// Problems
void sphere_problem(RealGenOptions opt, GAResults &results);
void rosenbrock_problem(RealGenOptions opt, GAResults &results);
void flatSurface_problem(RealGenOptions opt, GAResults &results);
void foxholes_problem(RealGenOptions opt, GAResults &results);

#endif
