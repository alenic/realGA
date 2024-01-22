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
#include "messages.h"

using namespace std;

class realGA {
public:
    realGA();
    ~realGA();
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
    //float getDiversity();  // -> TODO
    vector<RealChromosome> getPopulation();

    // Checking
    bool checkChromosome(RealChromosome &chromosome);
    void checkPopulation();
    string populationToString();

    // Initialization
    void popInitRandUniform();
    void popInitRandGaussian(float mean, float sigma);
    void popInitGaussianMutate(vector<float> &gene, float mutatioRate, float perc);
    void popInitSetChromosome(unsigned int index, RealChromosome &chromosome);
    void popInitSetPopulation(vector<RealChromosome>  &population);

    // Crossover
    void crossoverUniform(int indexA, int indexB, RealChromosome &c);
    void crossoverFixed(int indexA, int indexB, RealChromosome &c, int n);
    // Mutation
    void uniformMutate(RealChromosome &chromosome, float mutationRate, float perc);
    void gaussianMutate(RealChromosome &chromosome, float mutationRate, float perc);

    virtual void evolve();

protected:
    vector<RealChromosome> mPopulation;
    vector<RealChromosome> mNewPopulation;
    RealGAOptions mOptions;
    FitnessFunction *mFitnessFcn;
    vector<float> mFitnessValues;
    float mKthSmallestFitness;

    int mElitismNumber;

    // Selection Algorithm
    Selection *mSelectionAlgorithm;
    
    // State
    int mGeneration;
    // Gaussian mutation perc (change during generations)
    float mGaussianPerc;

    // Fill mFitnessValues vector
    void fillFitnessValues(vector<RealChromosome> &population);
};

#endif // REALGA_H
