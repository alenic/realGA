/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "realgenmultithread.h"

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
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);

    int selectedIndexA, selectedIndexB;
    int elitismIndex = (int)(mOptions.selectionElitismFactor * mOptions.populationSize);
    int k=0;

    // Fill fitness values vector
    fillFitnessValues();
    mSelectionAlgorithm->init(mFitnessValues);

    // Keep the 0:elitismIndex elements in the new population
    while (k < elitismIndex) {
        mNewPopulation[k] = mPopulation[k];
        ++k;
    }

     while(k < mOptions.populationSize) {
        // Selection
        switch(mOptions.selectionType) {
            case ROULETTE_WHEEL_SELECTION:
                // Choose index A and B from population according to roulette wheel selection strategy
                mSelectionAlgorithm->select(mFitnessValues, selectedIndexA, selectedIndexB);
                break;
            case TOURNAMENT_SELECTION:
                tournamentSelection(mOptions.selectionTournamentP, selectedIndexA, selectedIndexB);
                break;
        }

        // Crossover
        switch(mOptions.crossoverType) {
            case UNIFORM_CROSSOVER:
                crossoverUniform(selectedIndexA, selectedIndexB, offspring);
                break;
            case SINGLE_POINT_CROSSOVER:
                crossoverFixed(selectedIndexA, selectedIndexB, offspring, mOptions.crossoverIndex1);
                break;
        }
        

        // Mutation
        switch(mOptions.mutationType) {
            case UNIFORM_MUTATION:
                uniformMutate(offspring, mOptions.mutationRate, mGaussianPerc);
                break;
            case GAUSSIAN_MUTATION:
                gaussianMutate(offspring, mOptions.mutationRate, mOptions.mutationUniformPerc);
                break;
        }
        mNewPopulation[k] = offspring;
        ++k;
    }


    int interval = ceil((float)(mOptions.populationSize-elitismIndex)/(float)nThread);
    thread_params localThreadParam[nThread];

    for (int i = 0; i < nThread; ++i) {
        localThreadParam[i].startIndex = elitismIndex + interval*i;
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

    partial_sort(mNewPopulation.begin(), mNewPopulation.begin()+elitismIndex, mNewPopulation.end());

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        if (mGaussianPerc > mOptions.mutationGaussianPercMin) {
            mGaussianPerc = 1.0f - mOptions.mutationGaussianPercDelta*(float)mGeneration;
        } else {
            mGaussianPerc = mOptions.mutationGaussianPercMin;
        }
    }
    
    mPopulation = mNewPopulation;
    mGeneration++;
}
