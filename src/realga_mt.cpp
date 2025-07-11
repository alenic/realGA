#include "realga_mt.h"

RealGenMultithread::RealGenMultithread(unsigned int nThread) : RealGA()
{
#ifdef _WIN32
    localThread = new HANDLE[nThread];
#else
    localThread = new pthread_t[nThread];
#endif
}

RealGenMultithread::~RealGenMultithread()
{
    delete[] localThread;
}

#ifdef _WIN32
unsigned __stdcall RealGenMultithread::evaluatePopulationThread(void *params)
{
    struct thread_params *tp;
    tp = (struct thread_params *)params;

    for (int k = tp->startIndex; k < (tp->startIndex + tp->neval) && k < tp->ga->Np; ++k)
    {
        tp->ga->newPopulation[k].fitness = tp->ga->evalFitness(tp->ga->newPopulation[k]);
    }
    return 0;
}
#else

void *RealGenMultithread::evaluatePopulationThread(void *params)
{
    struct thread_params *tp;
    tp = (struct thread_params *)params;
    RealGenMultithread *ga = tp->ga;

    for (int k = tp->startIndex; k < tp->endIndex; ++k)
    {
        ga->mNewPopulation[k].fitness = ga->evalFitness(ga->mNewPopulation[k]);
    }
    pthread_exit(NULL);
}
#endif

void RealGenMultithread::evolve()
{
    // Allocate offspring (gene after crossover and mutation)
    RealChromosome offspring(mOptions.chromosomeSize);
    int selectedIndexA, selectedIndexB;
    size_t iter = 0;
    int countElite = 0;

    fillFitnessValues(mPopulation);
    // Find the kth smallest Fitness
    mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize - 1, mElitismNumber + 1);

    // Generate New Population

    // First, generate the new population (without evaluating fitness)
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
        // Do not evaluate fitness here, will be done in parallel below
        mNewPopulation[iter] = offspring;
        ++iter;
    }

    // Parallel fitness evaluation for non-elite chromosomes
    int nThread = this->nThread; // Assuming nThread is a member variable
    int interval = ceil((float)(mOptions.populationSize) / (float)nThread);
    thread_params localThreadParam[nThread];

    for (unsigned int i = 0; i < nThread; ++i)
    {
        localThreadParam[i].startIndex = interval * i;
        localThreadParam[i].endIndex = std::min(localThreadParam[i].startIndex + interval, (int)mOptions.populationSize);
        localThreadParam[i].ga = this;
#ifdef _WIN32
        unsigned threadID;
        localThread[i] = (HANDLE)_beginthreadex(0, 0, &RealGenMultithread::evaluatePopulationThread, (void *)&localThreadParam[i], 0, &threadID);
#else
        int rc;
        rc = pthread_create(&localThread[i], NULL, RealGenMultithread::evaluatePopulationThread, (void *)&localThreadParam[i]);
        REALGA_ERROR(rc, "Error:unable to create thread");
#endif
    }

    for (unsigned int i = 0; i < nThread; ++i)
    {
#ifdef _WIN32
        WaitForSingleObject(localThread[i], INFINITE);
#else
        pthread_join(localThread[i], NULL);
#endif
    }

    // After parallel fitness evaluation, handle duplicated fitness mutation if needed (serially)
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

    // Copy the new population
    mPopulation = mNewPopulation;
    mGeneration++;
}
