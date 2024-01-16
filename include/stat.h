/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#ifndef REALGEN_STAT_H
#define REALGEN_STAT_H
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>
#include <random>

#define MIN_SIGMA 1.0e-9f

class Stat {
    public:
    static float randUniform();
    static float randUniform(float lb, float ub);
    static float randGaussian(float m, float s);
    static int randIndex(int N);
    static void setSeed(unsigned int seed);
};

#endif // REALGEN_STAT_H
