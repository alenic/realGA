/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        General purpose static class, to manage all
        the pseudo-random generation.
---------------------------------------------
*/
#ifndef REALGA_STAT_H
#define REALGA_STAT_H
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>

#define MIN_SIGMA 1.0e-9f

class Stat {
    public:
    static float randUniform();
    static float randUniform(float lb, float ub);
    static float randGaussian(float m, float s);
    static int randIndex(int N);
    static void setSeed(unsigned int seed);
};

#endif // REALGA_STAT_H
