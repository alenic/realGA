#include "realga_mt.h"

#include <thread>
#include <fstream>
#include <iostream>
#include <unordered_set>


RealGAMultithread::RealGAMultithread(unsigned int nThread)
    : RealGA(), mNthread(nThread) {}

RealGAMultithread::~RealGAMultithread() {}

void RealGAMultithread::evolve()
{
    int n = mOptions.populationSize;
    int elitismNumber = (int)(mOptions.elitismFactor * n);

    // 1. Efficiency: Use nth_element to move the best individuals to the front
    // This is O(N) on average and handles ties perfectly.
    std::nth_element(mPopulation.begin(), 
                     mPopulation.begin() + elitismNumber, 
                     mPopulation.end(), 
                     [](const RealChromosome& a, const RealChromosome& b) {
                         return a.fitness < b.fitness;
                     });

    // 2. Use a Hash Set for O(1) duplicate lookups
    // We store fitness values as 'long long' or use a custom epsilon-based hash
    // for floating point safety, but a set of floats is the simplest upgrade.
    std::unordered_set<float> fitnessRegistry;
    int newPopIdx = 0;

    // 3. Transfer Elites
    for (; newPopIdx < elitismNumber; ++newPopIdx)
    {
        mNewPopulation[newPopIdx] = mPopulation[newPopIdx];
        if (mOptions.mutateDuplicatedFitness) {
            fitnessRegistry.insert(mNewPopulation[newPopIdx].fitness);
        }
    }

    // 4. Generate Offspring
    RealChromosome offspring(mOptions.chromosomeSize);
    while (newPopIdx < n)
    {
        int idxA, idxB;
        mSelectionAlgorithm->select(mFitnessValues, idxA, idxB);
        
        mCrossover->crossover(mPopulation[idxA], mPopulation[idxB], offspring);
        mMutation->mutate(offspring, mLB, mUB);
        offspring.fitness = evalFitness(offspring);

        // O(1) Duplicate Check
        if (mOptions.mutateDuplicatedFitness)
        {
            int attempts = 0;
            // If duplicate found, mutate again (limit attempts to avoid infinite loops)
            while (fitnessRegistry.find(offspring.fitness) != fitnessRegistry.end() && attempts < 10)
            {
                mMutation->mutate(offspring, mLB, mUB);
                offspring.fitness = evalFitness(offspring);
                attempts++;
            }
            fitnessRegistry.insert(offspring.fitness);
        }

        mNewPopulation[newPopIdx] = offspring;
        newPopIdx++;
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

    // 5. Update mutation cooling parameters
    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        mGaussianMutationPerc = fmax(mOptions.mutationGaussianPercMin, 1.0f - mOptions.mutationGaussianPercDelta * (float)mGeneration);
        mMutation->setMutationPercentage(mGaussianMutationPerc);
    }

    if (mOptions.mutationType == UNIFORM_MUTATION)
    {
        mUniformMutationPerc = fmax(mOptions.mutationUniformPercMin, 1.0f - mOptions.mutationUniformPercDelta * (float)mGeneration);
        mMutation->setMutationPercentage(mUniformMutationPerc);
    }

    mPopulation = mNewPopulation;
    
    // Crucial: Update the fitness value cache for the Selection Algorithm next turn
    fillFitnessValues(mPopulation); 
    
    mGeneration++;
}
