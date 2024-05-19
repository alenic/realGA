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
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Base class
class Crossover {
public:
    virtual ~Crossover() {}
    virtual void crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring) = 0;
};

// Uniform Crossover
class UniformCrossover: public Crossover {
public:
    UniformCrossover();
    ~UniformCrossover();
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