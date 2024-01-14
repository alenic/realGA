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
    void resetGaussianMutationPerc();

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
    void popInitRandGaussian(float mean, float sigma);
    void evalPopulationFitness();
    void popInitGaussianMutate(vector<float> &gene, float mutatioRate, float perc);
    // Selection
    void tournamentSelection(int p, int &indexA, int &indexB);
    void tournamentSelect(int p, int &index);
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

    Selection *mSelectionAlgorithm;
    vector<float> mFitnessValues;

    // State
    int mGeneration;
    // Gaussian mutation perc (change during generations)
    float mGaussianPerc;
};

#endif // REALGEN_H
