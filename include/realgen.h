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
#include "realchromosome.h"
#include "stat.h"
#include "realgenoptions.h"
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
    void setChromosomeInPopulation(unsigned int index, RealChromosome &chromosome);
    void setPopulation(vector<RealChromosome>  &population);

    void fillFitnessValues();
    void resetPopulation();
    void resetGaussianMutationSigma();

    // Getter
    RealChromosome getBestChromosome();
    int getGeneration();
    float evalFitness(const RealChromosome &);

    string populationToString();
    
    //float getDiversity();  // -> TODO
    void checkOptions();
    vector<RealChromosome> getPopulation();

    // Checking
    bool checkChromosome(RealChromosome &chromosome);
    void checkPopulation();

    // Initialization
    void popInitRandUniform();
    void evalPopulationFitness();
    void popInitGaussianMutate(vector<float> &gene, float mSigma);
    // Selection
    void tournamentSelection(int p, int &index1, int &index2);
    void tournamentSelect(int p, int &index);
    // Crossover
    void crossoverUniform(int index1, int index2, RealChromosome &c);
    void crossoverFixed(int index1, int index2, RealChromosome &c, int n);
    // Mutation
    void uniformMutate(RealChromosome &g, float perc);
    void gaussianMutate(RealChromosome &);

    virtual void evolve();

protected:
    vector <RealChromosome> mPopulation;
    vector <RealChromosome> mNewPopulation;
    RealGenOptions mOptions;
    FitnessFunction *mFitnessFcn;

    Selection *mSelectionAlgorithm;
    vector<float> mFitnessValues;

    // State
    int mGeneration;

    // Roulette wheel selection
    float mSumFitnessR;
    // Tournament selection
    int *mTourIndex;
    // Gaussian Mutation
    float *mSigma;
};

#endif // REALGEN_H
