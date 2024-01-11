/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#ifndef REALGENOPTIONS_H
#define REALGENOPTIONS_H
#include <string>
#include <iostream>
#include "realchromosome.h"
#include "fitnessfunction.h"

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=1, TOURNAMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=1, SINGLE_POINT_CROSSOVER, TWO_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=1, GAUSSIAN_MUTATION} MutationType;

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
    void setVerbose(bool value);
    void setSeed(bool seedValue);

    void setSelectionType(string value);
    void setSelectionTournamentP(int value);
    void setElitismFactor(float value);

    void setMutationType(string value);
    void setUniformMutationPercentage(float value);
    void setMutationRate(float rate);
    void setMutationGaussianScaleShrink(float scale, float shrink);

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
    bool verbose;

    //SelectionOpt
    SelectionType selectionType;
    int selectionTournamentP;
    float selectionElitismFactor;

    //CrossoverOpt
    CrossoverType crossoverType;
    unsigned int crossoverIndex1;
    unsigned int crossoverIndex2;

    // MutationOpt
    MutationType mutationType;
    float mutationUniformPerc;
    float mutationRate;
    float mutationGaussianScale;
    float mutationGaussianShrink;


};


#endif //REALGENOPTIONS_H
