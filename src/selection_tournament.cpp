#include "selection.h"

TournamentSelection::TournamentSelection(size_t populationSize)
{
    mTournamentSize = std::max(2, (int)((float)populationSize * 0.1f));
    mPopulationSize = populationSize;
    mSelectionProbability = 0.85f;
    mTournamentFitness.resize(mTournamentSize);
    mTournamentIndex.resize(mTournamentSize);
}

TournamentSelection::~TournamentSelection() = default;

void TournamentSelection::setTournamentSize(int tournamentSize)
{
    mTournamentSize = tournamentSize;
    mTournamentFitness.resize(mTournamentSize);
    mTournamentIndex.resize(mTournamentSize);
}

void TournamentSelection::setSelectionProbability(float selectionProbability)
{
    mSelectionProbability = selectionProbability;
}

void TournamentSelection::select(std::vector<float> &fitnessValues, int &indexA, int &indexB)
{
    indexA = tournament(fitnessValues);
    indexB = tournament(fitnessValues);

    while (indexA == indexB)
    {
        indexA = tournament(fitnessValues);
        indexB = tournament(fitnessValues);
    }
}

int TournamentSelection::tournament(std::vector<float> &fitnessValues)
{
    int i = 0;
    while (i < mTournamentSize)
    {
        int index = Stat::randIndex(mPopulationSize);
        bool match = false;
        for (int j = 0; j < i; j++)
        {
            if (mTournamentIndex[j] == index)
            {
                match = true;
                break;
            }
        }
        if (!match)
        {
            mTournamentIndex[i] = index;
            mTournamentFitness[i] = fitnessValues[index];
            ++i;
        }
    }

    int kth, localKthIndex, kthMinIndex;

    if (mSelectionProbability < 1.0f)
    {
        float chooseP = mSelectionProbability;
        for (kth = 1; kth < mTournamentSize; ++kth)
        {
            if (Stat::randUniform() < chooseP)
                break;
            chooseP *= (1.0f - mSelectionProbability);
        }

        localKthIndex = RALG::argKthSmallest(mTournamentFitness, 0, mTournamentSize - 1, kth);
        kthMinIndex = mTournamentIndex[localKthIndex];
    }
    else
    {
        localKthIndex = RALG::argMin(mTournamentFitness.data(), 0, mTournamentSize - 1);
        kthMinIndex = mTournamentIndex[localKthIndex];
    }

    return kthMinIndex;
}
