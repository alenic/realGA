/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#ifndef STAT_H_
#define STAT_H_
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>

#define MIN_SIGMA 1.0e-9f

using namespace std;

class Stat {
public:
    static float randUniform();
    static float randUniform(float lb, float ub);
    static float randGaussian(float m, float s);
    static int randIndex(int N);
    static void setSeed(unsigned int seed);
};

#endif // STAT_H_
