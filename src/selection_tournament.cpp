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
            mTournamentIndex[i]   = index;
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

        // build index order [0,1,2,...]
        static std::vector<int> order;
        order.resize(mTournamentSize);
        std::iota(order.begin(), order.end(), 0);

        int kthPos = std::clamp(kth - 1, 0, mTournamentSize - 1);

        std::nth_element(
            order.begin(),
            order.begin() + kthPos,
            order.end(),
            [&](int a, int b)
            {
                return mTournamentFitness[a] < mTournamentFitness[b];
            });

        localKthIndex = order[kthPos];
        kthMinIndex   = mTournamentIndex[localKthIndex];
    }
    else
    {
        auto it = std::min_element(
            mTournamentFitness.begin(),
            mTournamentFitness.begin() + mTournamentSize);

        localKthIndex = static_cast<int>(std::distance(mTournamentFitness.begin(), it));
        kthMinIndex   = mTournamentIndex[localKthIndex];
    }

    return kthMinIndex;
}
