#include "selection.h"
#include <algorithm>


RouletteWheelSelection::RouletteWheelSelection(int populationSize) {
    mCumulativeFitness.resize(populationSize);
}

RouletteWheelSelection::~RouletteWheelSelection() {
    
}

void RouletteWheelSelection::computeCumulativeValues(vector<float> &fitnessValues)
// call this function before the internal loop of evolve
{
    float cumSum = 0.0;
    unsigned int n = mCumulativeFitness.size();

    // Compute the maximum
    float maxValue = *std::max_element(fitnessValues.begin(), fitnessValues.end());
 
    // Compute cumulative vector (is inverted because we want to minimize)
    for(int i=0; i<n; i++) {
        float value = fitnessValues[i];
        cumSum += (maxValue - fitnessValues[i]);
        mCumulativeFitness[i] = cumSum;
    }
}

void RouletteWheelSelection::init(vector<float> &fitnessValues) {
    computeCumulativeValues(fitnessValues);
}

void RouletteWheelSelection::select(vector<float> &fitnessValues, int &index1, int &index2) {
    unsigned int n = mCumulativeFitness.size();
    float maxCumulative = mCumulativeFitness[n-1];
    float cumulative1, cumulative2;

    cumulative1 = maxCumulative * Stat::randUniform();
    cumulative2 = maxCumulative * Stat::randUniform();

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