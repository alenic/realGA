#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_
#include "realga.h"
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <sstream>

using namespace std;

enum TextColor
{
    TEXT_GREEN = 0,
    TEXT_YELLOW,
    TEXT_RED
};

void coutColor(const char *name, const char *s, TextColor color);

// Statistics Tests
class StatTest
{
private:
    float chiSquare(const int *observed, const float *expected, int bins);
    double erfcinv(double x);
    double chi_square_quantile_approx(int df, double alpha);
    bool mVerbose;

public:
    StatTest();
    StatTest(bool verbose);
    void test_randUniform(float lb, float ub);
    void test_randGaussian(float mean, float sigma);

    // void test_randIndex();
    // void test_randInteger();
    void test_smallSigmaGaussian();
};

// Realchromosome Tests
class RealChromosomeTest
{
public:
    RealChromosomeTest();
    void test_randUniform();
    void test_bound();
    void test_distanceTo();
};

// Selection Tests
class SelectionTests
{
public:
    SelectionTests();
    void test_roulette_select();
    void test_tournament_select();
};

void printDistribution(vector<float> &x, int bins, int nstars);

#endif
