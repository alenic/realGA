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

class Selection {
public:
    virtual void init(vector<float> &fitnessValues) = 0;
    virtual void select(vector<float> &fitnessValues, int &index1, int &index2) = 0;
};


class RouletteWheelSelection: public Selection {
public:
    RouletteWheelSelection(int populationSize);
    ~RouletteWheelSelection();

    void init(vector<float> &fitnessValues);
    void computeCumulativeValues(vector<float> &fitnessValues);

    void select(vector<float> &fitnessValues, int &index1, int &index2);
    int searchIndexBinarySearch(vector<float> arr, float x);
private:
    vector<float> mCumulativeFitness;
};

#endif // SELECTION_H