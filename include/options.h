/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        RealGAOptions defines all the option of your Genetic
        algorithm. You can set the selection, mutation and
        crossover strategy with relative parameters.
---------------------------------------------
*/
#ifndef REALGA_OPTIONS_H
#define REALGA_OPTIONS_H
#include <string>
#include <iostream>
#include "messages.h"
#include "chromosome.h"
#include "fitnessfunction.h"

using namespace std;

typedef enum
{
    ROULETTE_WHEEL_SELECTION = 1,
    TOURNAMENT_SELECTION
} SelectionType;
typedef enum
{
    UNIFORM_CROSSOVER = 1
} CrossoverType;
typedef enum
{
    UNIFORM_MUTATION = 1,
    GAUSSIAN_MUTATION
} MutationType;
typedef enum
{
    NO_VERBOSE = 0,
    SOFT_VERBOSE = 1,
    HARD_VERBOSE = 2
} VerboseType;

class RealGAOptions
{
public:
    RealGAOptions();
    ~RealGAOptions();

    void setPopulationSize(size_t np);
    void setChromosomeSize(size_t nx);
    void setLowerBounds(const vector<float> &lb);
    void setUpperBounds(const vector<float> &ub);
    void setBounds(const vector<float> &lb, const vector<float> &ub);
    void setVerbose(string value);
    void setSeed(unsigned int seedValue);
    void setElitismFactor(float value);
    void setMutateDuplicatedFitness(bool value);

    void setSelectionType(string value);
    void setSelectionTournamentSize(int value);
    void setSelectionTournamentProbability(float value);

    void setMutationType(string value);
    void setMutationRate(float value);
    void setMutationGaussianPerc(float percDelta, float percMin);
    void setMutationUniformPerc(float percDelta, float percMin);

    void setCrossoverType(string value);
    void setSinglePointCrossoverIndex(int value);

    void checkOptions();

    // Basic options
    size_t populationSize;
    size_t chromosomeSize;
    vector<float> lowerBounds;
    vector<float> upperBounds;
    int seed;
    VerboseType verbose;

    float elitismFactor;
    bool mutateDuplicatedFitness;

    // SelectionOpt
    SelectionType selectionType;
    size_t selectionTournamentSize;
    float selectionTournamentProbability;

    // CrossoverOpt
    CrossoverType crossoverType;
    int crossoverindexA;

    // MutationOpt
    MutationType mutationType;
    float mutationRate;

    float mutationUniformPercDelta;
    float mutationUniformPercMin;

    float mutationGaussianPercDelta;
    float mutationGaussianPercMin;
};

#endif // REALGA_OPTIONS_H
