#include "selection.h"


RouletteWheelSelection::RouletteWheelSelection(int populationSize) {
    mType = STOCHASTIC_ACCEPTANCE;
    mNormalizedFitness.resize(populationSize);
}

RouletteWheelSelection::RouletteWheelSelection(int populationSize, RouletteType type) :  RouletteWheelSelection(populationSize) {
    mType = type;

    switch(type) {
        case STOCHASTIC_ACCEPTANCE:
            mNormalizedFitness.resize(populationSize);
        break;
        case BIN_SEARCH:
            mCumulativeFitness.resize(populationSize);
        break;
    }
}

RouletteWheelSelection::~RouletteWheelSelection() {}



void RouletteWheelSelection::computeCumulativeValues(vector<float> &fitnessValues)
// call this function before the internal loop of evolve
{
    unsigned int n = mCumulativeFitness.size();
    float minValue, maxValue;

    // Compute min max
    RALG::minmax(fitnessValues, minValue, maxValue);

    float delta = maxValue - minValue;
    mCumSum = 0.0;
    for(int i=0; i<n; i++) {
        // cumsum += 1 - minmax normalization
        mCumSum += 1.0f - (fitnessValues[i] - minValue + 1.0f) / delta;
        mCumulativeFitness[i] = mCumSum;
    }
}

void RouletteWheelSelection::init(vector<float> &fitnessValues) {
    switch(mType) {
        case STOCHASTIC_ACCEPTANCE:
            RALG::minmax(fitnessValues, mMinFitnessValue, mMaxFitnessValue);
            for(int i=0; i< fitnessValues.size(); i++) {
                // weakest chromosome taken with probability 0.01
                mNormalizedFitness[i] = (1.01f - (fitnessValues[i] - mMinFitnessValue) / (mMaxFitnessValue - mMinFitnessValue)) / 1.01f;
            }

        break;
        case BIN_SEARCH:
            computeCumulativeValues(fitnessValues);
        break;
    }
    
}

void RouletteWheelSelection::selectStochasticAcceptance(int &indexA, int &indexB) {
    int n = mNormalizedFitness.size();
    while(1) {
        indexA = Stat::randIndex(n);
        if (Stat::randUniform() < mNormalizedFitness[indexA]) {
            break;
        }
    }

    while(1) {
        indexB = Stat::randIndex(n);
        if(indexB == indexA) continue;
        if (Stat::randUniform() < mNormalizedFitness[indexB]) {
            break;
        }
    }
}

void RouletteWheelSelection::select(vector<float> &fitnessValues, int &indexA, int &indexB) {
    unsigned int n = fitnessValues.size();
    switch(mType) {
        case STOCHASTIC_ACCEPTANCE:
        selectStochasticAcceptance(indexA, indexB);

        break;
        case BIN_SEARCH:
            float cumulative1, cumulative2;
            
            cumulative1 = mCumSum * Stat::randUniform();
            cumulative2 = mCumSum * Stat::randUniform();

            indexA = RALG::searchIndexBinarySearch(mCumulativeFitness, cumulative1, 0, n-1);
            indexB = RALG::searchIndexBinarySearch(mCumulativeFitness, cumulative2, 0, n-1);
            if(indexA == indexB) {
                if(indexA != n-1) {
                    indexB = indexA + 1;
                } else {
                    indexB = indexA - 1;
                }
            }
        break;
    }
}

