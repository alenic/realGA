#ifndef REALGENOPTIONS_H
#define REALGENOPTIONS_H
#include <string>
#include <iostream>
#include "realgenotype.h"
#include "fitnessfunction.h"

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=0, TOURNAMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=0, SINGLE_POINT_CROSSOVER, TWO_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=0, GAUSSIAN_MUTATION} MutationType;

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

    void setSelectionType(string value);
    void setSelectionTournamentP(int value);
    void setSorting(bool value);
    void setElitismFactor(float value);

    void setMutationType(string value);
    void setUniformMutationPercentage(float value);
    void setMutationRate(float value);
    void setMutationGaussianScaleShrink(float scale, float shrink);

    void setCrossoverType(string value);
    void setSinglePointCrossoverIndex(int value);
    void setTwoPointCrossoverIndexes(int i1, int i2);

public:
    int populationSize;
    //SelectionOpt
    SelectionType selectionType;
    int selectionTournamentP;
    bool selectionSorting;
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

    int chromosomeSize;
    vector<float> lowerBounds;
    vector<float> upperBounds;
    int maxGenerations;
};


#endif //REALGENOPTIONS_H
