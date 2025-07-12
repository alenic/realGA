#include "realga_mt.h"

#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>

RealGenMultithread::RealGenMultithread(unsigned int nThread)
    : RealGA(), mNthread(nThread) {}

RealGenMultithread::~RealGenMultithread() {}

void RealGenMultithread::evolve()
{
    RealChromosome offspring(mOptions.chromosomeSize);
    int selectedIndexA, selectedIndexB;
    size_t iter = 0;
    int countElite = 0;

    fillFitnessValues(mPopulation);
    mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize - 1, mElitismNumber + 1);

    // Generate new population (without evaluating fitness)
    while (iter < mOptions.populationSize)
    {
        if ((mFitnessValues[iter] <= mKthSmallestFitness) && (countElite < mElitismNumber))
        {
            mNewPopulation[iter] = mPopulation[iter];
            ++iter;
            ++countElite;
            continue;
        }

        mSelectionAlgorithm->select(mFitnessValues, selectedIndexA, selectedIndexB);
        mCrossover->crossover(mPopulation[selectedIndexA], mPopulation[selectedIndexB], offspring);
        mMutation->mutate(offspring, mLB, mUB);
        mNewPopulation[iter] = offspring;
        mNewPopulation[iter].fitness = std::numeric_limits<float>::max();
        ; // Evaluate only non-elites fitness
        ++iter;
    }

    // Parallel fitness evaluation
    int chunkSize = std::ceil(static_cast<float>(mOptions.populationSize) / mNthread);
    std::vector<std::thread> threads;

    for (unsigned int t = 0; t < mNthread; ++t)
    {
        int start = t * chunkSize;
        int end = std::min(start + chunkSize, (int)mOptions.populationSize);

        threads.emplace_back([this, start, end]()
                             {
            for (int k = start; k < end; ++k)
            {
                if (mNewPopulation[k].fitness == std::numeric_limits<float>::max())  // Only evaluate non-elites
                {
                    mNewPopulation[k].fitness = evalFitness(mNewPopulation[k]);
                }
            } });
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    // Handle duplicated fitness mutation (serial)
    if (mOptions.mutateDuplicatedFitness)
    {
        for (size_t i = 0; i < mOptions.populationSize; ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                if (fabs(mNewPopulation[i].fitness - mNewPopulation[j].fitness) < 1.0e-12)
                {
                    mMutation->mutate(mNewPopulation[i], mLB, mUB);
                    mNewPopulation[i].fitness = evalFitness(mNewPopulation[i]);
                }
            }
        }
    }

    // Update mutation percentages if adaptive
    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        mGaussianMutationPerc = std::max(mOptions.mutationGaussianPercMin,
                                         1.0f - mOptions.mutationGaussianPercDelta * static_cast<float>(mGeneration));
        mMutation->setMutationPercentage(mGaussianMutationPerc);
    }

    if (mOptions.mutationType == UNIFORM_MUTATION)
    {
        mUniformMutationPerc = std::max(mOptions.mutationUniformPercMin,
                                        1.0f - mOptions.mutationUniformPercDelta * static_cast<float>(mGeneration));
        mMutation->setMutationPercentage(mUniformMutationPerc);
    }

    mPopulation = mNewPopulation;
    mGeneration++;
}
