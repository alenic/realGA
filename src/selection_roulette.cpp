#include "selection.h"

RouletteWheelSelection::RouletteWheelSelection(size_t populationSize)
{
    mNormalizedFitness.resize(populationSize);
}

RouletteWheelSelection::~RouletteWheelSelection() {}

void RouletteWheelSelection::normalize(vector<float> &fitnessValues)
{
    RALG::minmax(fitnessValues, mMinFitnessValue, mMaxFitnessValue);

    for (size_t i = 0; i < fitnessValues.size(); i++)
    {
        // weakest chromosome taken with probability 0.01
        if (mMaxFitnessValue == mMinFitnessValue)
        {
            mNormalizedFitness[i] = 1.0f;
        }
        else
        {
            mNormalizedFitness[i] = (1.01f - (fitnessValues[i] - mMinFitnessValue) / (mMaxFitnessValue - mMinFitnessValue)) / 1.01f;
        }
    }
}

void RouletteWheelSelection::selectStochasticAcceptance(int &indexA, int &indexB)
{
    int n = mNormalizedFitness.size();
    while (1)
    {
        indexA = Stat::randIndex(n);
        if (Stat::randUniform() < mNormalizedFitness[indexA])
        {
            break;
        }
    }

    while (1)
    {
        indexB = Stat::randIndex(n);
        if (indexB == indexA)
            continue;
        if (Stat::randUniform() < mNormalizedFitness[indexB])
        {
            break;
        }
    }
}

void RouletteWheelSelection::select(vector<float> &fitnessValues, int &indexA, int &indexB)
{
    normalize(fitnessValues);
    selectStochasticAcceptance(indexA, indexB);
}
