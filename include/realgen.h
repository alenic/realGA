/*

realgen.h : genetic algorithm core

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

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
#include "realgenotype.h"
#include "stat.h"
#include "realgenoptions.h"
#include "fitnessfunction.h"

using namespace std;

class RealGen {
public:
    RealGen(RealGenOptions &opt);
    ~RealGen();
    // Setter
    void setFitnessFunction(FitnessFunction *f);
    void setSorting(bool);
    void setMutationRate(float);
    void setElitismFactor(float);
    void setPopulationSize(int);
    void setOptions(RealGenOptions &opt);
    void setMaxGenerations(int);
    void setSeed(unsigned int);
    void setVerbose(bool value);

    void setPopulation(int index, RealGenotype &chromosome);
    void setPopulation(vector<RealGenotype>  &new_population);
    // getter
    RealGenotype getBestChromosome();
    int getGeneration();
    double evalFitness(const RealGenotype &);
    double getMeanFitness();
    double getMinFitness();
    string populationToString();
    double getDiversity();
    void checkOptions();
    vector<RealGenotype> getPopulation();

    // For debug
    bool checkChromosome(RealGenotype &chromosome);
    void checkPopulation();

    //  ================= Initialization =================================
    void initRandom();
    void evalPopulationFitness();
    void initMutate(vector<float> &gene, float mSigma);
    // ================= Selection =================================
    void rouletteWheelSelection(int &index1, int &index2);
    void sumFitnessRoulette();
    void rouletteWheel(int &index, float stop);
    void tournamentSelection(int p, int &index1, int &index2);
    void tournamentSelect(int p, int &index);
    //==================================== Crossover ==================
    void crossoverUniform(int index1, int index2, RealGenotype &c);
    void crossoverFixed(int index1, int index2, RealGenotype &c, int n);
    //===================================== Mutation ======================
    void uniformMutate(RealGenotype &g, float perc);
    void gaussianMutate(RealGenotype &);
    // ====================================================================
    virtual void evolve();

protected:
    vector <RealGenotype> mPopulation;
    vector <RealGenotype> mNewPopulation;
    RealGenOptions mOptions;
    int mNp;
    int mNx;

    FitnessFunction *mFitnessFcn;
    int mGeneration;

    int mMaxGenerations;  // Maximum number of generations (default: 100*number of variables)
    double mSumFitnessR; // Only for roulette wheel selection
    int *mTourIndex; // Only for tournament selection
    float *mSigma;

    double mMinFitness;
    int mIndexMinFitness;
    double mMaxFitness;
    int mIndexMaxFitness;
    int mVerbose;

    void evalMinFitness();
    void evalMaxFitness();
};

#endif // REALGEN_H
