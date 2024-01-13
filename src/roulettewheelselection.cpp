#include "selection.h"


RouletteWheelSelection::RouletteWheelSelection(int populationSize) {
    mCumulativeFitness.resize(populationSize);
    mCumSum = 0;
}

RouletteWheelSelection::~RouletteWheelSelection() {}

void RouletteWheelSelection::computeCumulativeValues(vector<float> &fitnessValues)
// call this function before the internal loop of evolve
{
    unsigned int n = mCumulativeFitness.size();
    float minValue = fitnessValues[0];
    float maxValue = fitnessValues[0];
    // compute minimum and maximum
    for(int i=0; i<n; i++) {
        float v = fitnessValues[i];
        if(v < minValue) minValue = v;
        if(v > maxValue) maxValue = v;
    }

    float delta = maxValue - minValue;
    mCumSum = 0.0;
    for(int i=0; i<n; i++) {
        // cumsum += 1 - minmax normalization
        mCumSum += 1.0f - (fitnessValues[i] - minValue) / delta;
        mCumulativeFitness[i] = mCumSum;
    }
}

void RouletteWheelSelection::init(vector<float> &fitnessValues) {
    computeCumulativeValues(fitnessValues);
}

void RouletteWheelSelection::select(vector<float> &fitnessValues, int &index1, int &index2) {
    unsigned int n = mCumulativeFitness.size();
    float cumulative1, cumulative2;
    
    cumulative1 = mCumSum * Stat::randUniform();
    cumulative2 = mCumSum * Stat::randUniform();

    index1 = searchIndexBinarySearch(mCumulativeFitness, cumulative1);
    index2 = searchIndexBinarySearch(mCumulativeFitness, cumulative2);

    if(index1 == index2) {
        if(index1 != n-1) {
            index2 = index1 + 1;
        } else {
            index2 = index1 - 1;
        }
    }
}

// An iterative binary search function.
int RouletteWheelSelection::searchIndexBinarySearch(vector<float> arr, float x)
{
    int l=0, r=mCumulativeFitness.size()-1;
    int m = 0;
    while (l < r) {
        m = (r + l) / 2;
        if (l==r) {
            return m;
        }
        // modified binary search to find the right index
        if(r == l+1) {
            if (x > arr[l] && x < arr[r]) return l;
            if (x > arr[r]) return r;
        }
        // Check if x is present at mid
        if (arr[m] == x)
            return m;
        // If x greater, ignore left half
        if (arr[m] < x)
            l = m;
        // If x is smaller, ignore right half
        else
            r = m;
    }
    // If we reach here, then element was not present
    return m;
}