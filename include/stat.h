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

#include <cstddef> // for size_t

class Stat
{
public:
    // Generate a uniform random float in [0, 1)
    static float randUniform();

    // Generate a uniform random float in [lb, ub)
    static float randUniform(float lb, float ub);

    // Generate a random integer index in [0, N)
    static int randIndex(int N);

    // Generate a Gaussian-distributed float with mean and sigma
    static float randGaussian(float mean, float sigma);

    // Set fixed seed for reproducibility
    static void setSeed(unsigned int seed);

    // Set seed from hardware random device
    static void setSeedFromDevice();

    // Templated function for generating random integers in [min_val, max_val]
    static int randInteger(int min_val, int max_val);
};

#endif // REALGA_STAT_H
