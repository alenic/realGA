/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#ifndef REALGEN_OPTIONS_H
#define REALGEN_OPTIONS_H
#include <string>
#include <iostream>
#include "chromosome.h"
#include "fitnessfunction.h"

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=1, TOURNAMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=1, SINGLE_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=1, GAUSSIAN_MUTATION} MutationType;
typedef enum { NO_VERBOSE=0, SOFT_VERBOSE=1, HARD_VERBOSE=2} VerboseType;
/*
  Class used for option settings
*/
class RealGenOptions {
public:
    RealGenOptions();
    ~RealGenOptions();

    void setPopulationSize(int np);
    void setChromosomeSize(int nx);
    void setLowerBounds(const vector<float> &lb);
    void setUpperBounds(const vector<float> &ub);
    void setBounds(const vector<float> &lb, const vector<float> &ub);
    void setMaxGenerations(int value);
    void setVerbose(string value);
    void setSeed(bool seedValue);

    void setSelectionType(string value);
    void setSelectionTournamentSize(int value);
    void setSelectionTournamentProbability(float value);
    void setElitismFactor(float value);

    void setMutationType(string value);
    void setUniformMutationPercentage(float value);
    void setMutationRate(float rate);
    void setMutationGaussianPerc(float percDelta, float percMin);

    void setCrossoverType(string value);
    void setSinglePointCrossoverIndex(int value);
    void setTwoPointCrossoverIndexes(int i1, int i2);

    // Basic options
    int populationSize;
    int chromosomeSize;
    vector<float> lowerBounds;
    vector<float> upperBounds;
    int maxGenerations;
    int seed;
    VerboseType verbose;

    float selectionElitismFactor;

    //SelectionOpt
    SelectionType selectionType;
    int selectionTournamentSize;
    float selectionTournamentProbability;
    
    //CrossoverOpt
    CrossoverType crossoverType;
    unsigned int crossoverindexA;
    unsigned int crossoverindexB;

    // MutationOpt
    MutationType mutationType;
    float mutationRate;
    // Uniform mutation
    float mutationUniformPerc;
    // Gaussian mutation
    float mutationGaussianPercDelta;
    float mutationGaussianPercMin;


};


#endif //REALGEN_OPTIONS_H
