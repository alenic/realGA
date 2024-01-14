/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#ifndef REALGEN_H
#define REALGEN_H
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
#include "options.h"
#include "fitnessfunction.h"
#include "selection.h"

using namespace std;

class RealGen {
public:
    RealGen();
    ~RealGen();
    // Setter
    void init(RealGenOptions &opt, FitnessFunction *func, bool keepState);
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
    void checkOptions();
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
    vector <RealChromosome> mPopulation;
    vector <RealChromosome> mNewPopulation;
    RealGenOptions mOptions;
    FitnessFunction *mFitnessFcn;
    vector<float> mFitnessValues;

    int mElitismIndex;

    // Selection Algorithm
    Selection *mSelectionAlgorithm;
    
    // State
    int mGeneration;
    // Gaussian mutation perc (change during generations)
    float mGaussianPerc;

    // Fill mFitnessValues vector
    void fillFitnessValues();
};

#endif // REALGEN_H
