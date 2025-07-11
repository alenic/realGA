/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This class is the implementation of selection
        strategies.
---------------------------------------------
*/
#ifndef REALGA_SELECTION_H
#define REALGA_SELECTION_H
#include "stat.h"
#include "options.h"
#include "algorithms.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Abstract class
class Selection
{
public:
    virtual ~Selection() {}
    virtual void select(vector<float> &fitnessValues, int &indexA, int &indexB) = 0;
};

// Roulette-Wheel selection strategy
class RouletteWheelSelection : public Selection
{
public:
    RouletteWheelSelection(size_t populationSize);
    ~RouletteWheelSelection();

    void normalize(vector<float> &fitnessValues);
    void select(vector<float> &fitnessValues, int &indexA, int &indexB);

private:
    // For stochastic acceptance
    vector<float> mNormalizedFitness;
    float mMinFitnessValue;
    float mMaxFitnessValue;
    void selectStochasticAcceptance(int &indexA, int &indexB);
};

// Tournament selection strategy
class TournamentSelection : public Selection
{
public:
    TournamentSelection(size_t populationSize);
    ~TournamentSelection();

    void setTournamentSize(int tournamentSize);
    void setSelectionProbability(float selectionProbability);

    void select(vector<float> &fitnessValues, int &indexA, int &indexB);
    int tournament(vector<float> &fitnessValues);

private:
    int mTournamentSize;
    size_t mPopulationSize;
    // Probability of select the winner in a tournament
    float mSelectionProbability;

    // Local vectors for the tournaments
    vector<float> mTournamentFitness;
    vector<int> mTournamentIndex;
};

#endif // REALGA_SELECTION_H