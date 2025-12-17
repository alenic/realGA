/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This class is the implementation of crossover strategies
---------------------------------------------
*/
#ifndef REALGA_CROSSOVER_H
#define REALGA_CROSSOVER_H
#include "stat.h"
#include "options.h"
#include "messages.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Base class
class Crossover {
public:
    Crossover(size_t chromosomeSize): mChromosomeSize(chromosomeSize) {}
    virtual ~Crossover() {}
    virtual void crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring) = 0;
protected:
    size_t mChromosomeSize;
};

// Uniform Crossover
class UniformCrossover: public Crossover {
public:
    UniformCrossover(size_t chromosomeSize);
    ~UniformCrossover();
    void crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring);
};


// Linear Crossover
class LinearCrossover: public Crossover {
public:
    LinearCrossover(size_t chromosomeSize);
    ~LinearCrossover();
    void crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring);
};


// Fixed Crossover
/*
class FixedCrossover: public Crossover {
public:
    FixedCrossover(vector<int> &switchIndices);
    ~FixedCrossover();
    void crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring);
private:
    vector<int> mSwitchIndices;
};
*/
#endif // REALGA_CROSSOVER_H