/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This is the core class, where you can call
        the evolve method to create new populations.
        You have to provide the options of your Genetic
        Algorithm to change selection, mutation and
        crossover strategies.
---------------------------------------------
*/
#ifndef REALGA_H
#define REALGA_H
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include "chromosome.h"
#include "stat.h"
#include "algorithms.h"
#include "options.h"
#include "fitnessfunction.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "messages.h"

class RealGA
{
public:
    RealGA();
    virtual ~RealGA();
    // Setter
    void restart();
    void init(RealGAOptions &opt, FitnessFunction *func, bool keepState);
    void setFitnessFunction(FitnessFunction *f);

    void resetPopulation();
    void resetGaussianMutationPerc();

    float evalFitness(const RealChromosome &);

    // Getter
    RealChromosome getBestChromosome();
    int getGeneration();
    // float getDiversity();  // -> TODO
    const vector<RealChromosome> &getPopulation() const;

    // Debugging
    void checkPopulation();
    string populationToString();

    // Initialization
    void popInitRandUniform();
    void popInitRandGaussian(float mean, float sigma);
    void popInitGaussianMutate(vector<float> &gene, float mutatioRate, float mutationPerc);
    void popInitSetChromosome(unsigned int index, RealChromosome &chromosome);
    void popInitSetPopulation(vector<RealChromosome> &population);
    void evaluatePopulationFitness();
    virtual void evolve();

protected:
    vector<float> mLB;
    vector<float> mUB;

    vector<RealChromosome> mPopulation;
    vector<RealChromosome> mNewPopulation;
    RealGAOptions mOptions;
    FitnessFunction *mFitnessFcn;
    vector<float> mFitnessValues;

    // Selection Algorithm
    Selection *mSelectionAlgorithm;

    // Crossover
    Crossover *mCrossover;

    // Mutation
    Mutation *mMutation;

    // State
    int mGeneration;
    // Gaussian mutation perc (change during generations)
    float mGaussianMutationPerc;
    float mUniformMutationPerc;

    // Fill mFitnessValues vector
    void fillFitnessValues(vector<RealChromosome> &population);
};

#endif // REALGA_H
