/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic

*/
#ifndef SELECTION_H
#define SELECTION_H
#include "stat.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Abstract class
class Selection {
public:
    virtual void init(vector<float> &fitnessValues) = 0;
    virtual void select(vector<float> &fitnessValues, int &indexA, int &indexB) = 0;
};

// Roulette-Wheel selection strategy
class RouletteWheelSelection: public Selection {
public:
    RouletteWheelSelection(int populationSize);
    ~RouletteWheelSelection();

    void init(vector<float> &fitnessValues);
    void computeCumulativeValues(vector<float> &fitnessValues);

    void select(vector<float> &fitnessValues, int &indexA, int &indexB);
    int searchIndexBinarySearch(vector<float> arr, float x);
private:
    vector<float> mCumulativeFitness;

    float mCumSum;
};


// Tournament selection strategy
class TournamentSelection: public Selection {
public:
    TournamentSelection();
    TournamentSelection(int mTorunamentSize);
    ~TournamentSelection();

    void init(vector<float> &fitnessValues);
    void select(vector<float> &fitnessValues, int &indexA, int &indexB);

private:
    int mTorunamentSize;
    int mPopulationSize;
};


#endif // SELECTION_H