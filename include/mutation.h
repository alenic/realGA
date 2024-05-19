/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This class is the implementation of mutation strategies
---------------------------------------------
*/
#ifndef REALGA_MUTATION_H
#define REALGA_MUTATION_H
#include "stat.h"
#include "options.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Base class
class Mutation {
public:
    Mutation();
    Mutation(float mutationRate, float mutationPerc);
    virtual ~Mutation();
    virtual void mutate(RealChromosome &x, vector<float> &lb, vector<float> &ub) = 0;
    void setMutationRate(float mutationRate);
    void setMutationPercentage(float mutationPerc);
    float getMutationRate();
    float getMutationPercentage();
protected:
    float mMutationRate;
    float mMutationPerc;
};

// Uniform Mutation
class UniformMutation: public Mutation {
public:
    UniformMutation();
    UniformMutation(float mutationRate, float mutationPerc);
    ~UniformMutation();
    void mutate(RealChromosome &x, vector<float> &lb, vector<float> &ub);
};

// Gaussian Mutation
class GaussianMutation: public Mutation {
public:
    GaussianMutation();
    GaussianMutation(float mutationRate, float mutationPerc);
    ~GaussianMutation();
    void mutate(RealChromosome &x, vector<float> &lb, vector<float> &ub);
};


#endif // REALGA_MUTATION_H