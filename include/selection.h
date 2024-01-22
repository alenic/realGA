/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic

*/
#ifndef REALGEN_SELECTION_H
#define REALGEN_SELECTION_H
#include "stat.h"
#include "options.h"
#include "algorithms.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Abstract class
class Selection {
public:
    virtual ~Selection() {}
    virtual void init(vector<float> &fitnessValues) = 0;
    virtual void select(vector<float> &fitnessValues, int &indexA, int &indexB) = 0;
};

// Roulette-Wheel selection strategy
class RouletteWheelSelection: public Selection {
public:
    RouletteWheelSelection(int populationSize);
    ~RouletteWheelSelection();

    void init(vector<float> &fitnessValues);
    void select(vector<float> &fitnessValues, int &indexA, int &indexB);
private:
    // For stochastic acceptance
    vector<float> mNormalizedFitness;
    float mMinFitnessValue;
    float mMaxFitnessValue;
    void selectStochasticAcceptance(int &indexA, int &indexB);
};


// Tournament selection strategy
class TournamentSelection: public Selection {
public:
    TournamentSelection(int populationSize);
    ~TournamentSelection();

    void setTournamentSize(int tournamentSize);
    void setSelectionProbability(float selectionProbability);

    void init(vector<float> &fitnessValues);
    void select(vector<float> &fitnessValues, int &indexA, int &indexB);
    int tournament(vector<float> &fitnessValues);
private:
    int mTournamentSize;
    int mPopulationSize;
    // Probability of select the winner in a tournament
    float mSelectionProbability;

    // Local vectors for the tournaments
    float *mTournamentFitness;
    int *mTournamentIndex;
};


#endif // REALGEN_SELECTION_H