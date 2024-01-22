/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "realgen_mt.h"

// Constructors
RealGenMultithread::RealGenMultithread(unsigned int nThread) : RealGen() {
#ifdef _WIN32
    localThread = new HANDLE[nThread];
#else
    localThread = new pthread_t[nThread];
#endif
}


RealGenMultithread::~RealGenMultithread() {
    delete []localThread;
}

#ifdef _WIN32
unsigned __stdcall RealGenMultithread::evaluatePopulationThread(void *params)
{
    struct thread_params *tp;
    tp = (struct thread_params *) params;

    for (int k = tp->startIndex; k<(tp->startIndex + tp->neval) && k < tp->ga->Np; ++k) {
        tp->ga->newPopulation[k].fitness = tp->ga->evalFitness(tp->ga->newPopulation[k]);
    }
    return 0;
}
#else
void *RealGenMultithread::evaluatePopulationThread(void *params) {
    struct thread_params *tp;
    tp = (struct thread_params *) params;
    RealGenMultithread * ga = tp->ga;

    for (int k = tp->startIndex; k < tp->endIndex; ++k) {
        ga->mNewPopulation[k].fitness = ga->evalFitness(ga->mNewPopulation[k]);
    }
    pthread_exit(NULL);
}
#endif



void RealGenMultithread::evolve() {
    // Allocate offspring (a new gene)
    RealChromosome offspring(mOptions.chromosomeSize);
    int selectedIndexA, selectedIndexB;
    int k=0;
    int countElite=0;

    // fill mFitnessValues to accelerate some functions
    fillFitnessValues(mPopulation);
    // Find the kth smallest Fitness value
    mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize-1, mElitismNumber);

    mSelectionAlgorithm->init(mFitnessValues);
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);

    // Generate New Population
    while(k < mOptions.populationSize) {

        if((mFitnessValues[k] <= mKthSmallestFitness) && (countElite <= mElitismNumber)) {
            mNewPopulation[k] = mPopulation[k];
            ++k;
            ++countElite;
            continue;
        }

        // Selection
        mSelectionAlgorithm->select(mFitnessValues, selectedIndexA, selectedIndexB);

        // Crossover
        switch(mOptions.crossoverType) {
            case UNIFORM_CROSSOVER:
                crossoverUniform(selectedIndexA, selectedIndexB, offspring);
                break;
            case SINGLE_POINT_CROSSOVER:
                crossoverFixed(selectedIndexA, selectedIndexB, offspring, mOptions.crossoverindexA);
                break;
        }
        
        // Mutation
        switch(mOptions.mutationType) {
            case UNIFORM_MUTATION:
                uniformMutate(offspring, mOptions.mutationRate, mOptions.mutationUniformPerc);
                break;
            case GAUSSIAN_MUTATION:
                gaussianMutate(offspring, mOptions.mutationRate, mGaussianPerc);
                break;
        }

        mNewPopulation[k] = offspring;
        ++k;
    }


    int interval = ceil((float)(mOptions.populationSize)/(float)nThread);
    thread_params localThreadParam[nThread];

    for (int i = 0; i < nThread; ++i) {
        localThreadParam[i].startIndex = interval*i;
        localThreadParam[i].endIndex = min(localThreadParam[i].startIndex + interval, (int)mOptions.populationSize);
        localThreadParam[i].ga = this;


#ifdef _WIN32
        unsigned threadID;
        localThread[i] = (HANDLE)_beginthreadex(0, 0, &RealGenMultithread::evaluatePopulationThread, (void *)&localThreadParam[i], 0, &threadID);
#else
        int rc;
        rc = pthread_create(&localThread[i], NULL, RealGenMultithread::evaluatePopulationThread, (void *)&localThreadParam[i]);
        if (rc) {
            cerr << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
#endif
    }

    for(int i=0; i<nThread; ++i) {
#ifdef _WIN32
        WaitForSingleObject(localThread[i], INFINITE);
#else
        pthread_join(localThread[i], NULL);
#endif
    }

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        if (mGaussianPerc > mOptions.mutationGaussianPercMin) {
            mGaussianPerc = 1.0f - mOptions.mutationGaussianPercDelta*(float)mGeneration;
        } else {
            mGaussianPerc = mOptions.mutationGaussianPercMin;
        }
    }

    for(int i=0; i<mPopulation.size(); i++) {
        mPopulation[i] = mNewPopulation[i];
    }
    
    mGeneration++;
}
