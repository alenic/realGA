#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_
#include "realga.h"
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
    float uniformChiSquared(int *a, int n, int N);
public:
    StatTest();
    void test_uniformDist();
    void test_gaussianDist(int mean, int stDev);
};

// Algorithms Tests
class RALGTest {
public:
    RALGTest(){};
    void test_argKthSmallest();
    void test_searchIndexBinarySearch();
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
    void test_roulette_select();
    void test_tournament_select();
};


void printDistribution(vector<float> &x, int bins, int nstars);

#endif
