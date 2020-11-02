#include "realgenmultithread.h"

// Constructors
RealGenMultithread::RealGenMultithread(RealGenOptions &opt, unsigned int nt) : RealGen(opt) {
    nThread = nt;
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
    RealGenotype offspring(mNx);
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);

    int index1=0, index2=1;
    int elitismIndex = (int)(mOptions.selectionElitismFactor*mNp);
    int k=0;

    if(mOptions.selectionType == ROULETTE_WHEEL_SELECTION) {
        sumFitnessRoulette();
    }

    if (mOptions.selectionSorting) {
        while (k < elitismIndex) {
            mNewPopulation[k] = mPopulation[k];
            ++k;
        }
    } else {
        // Keep only the best one
        mNewPopulation[k] = mPopulation[mIndexMinFitness];
        ++k;
    }

    while(k < mNp) {
        // Selection
        switch(mOptions.selectionType) {
        case ROULETTE_WHEEL_SELECTION:
            rouletteWheelSelection(index1, index2);
            break;
        case TOURNAMENT_SELECTION:
            tournamentSelection(mOptions.selectionTournamentP, index1, index2);
            break;
        }

        // Crossover
        crossoverUniform(index1, index2, offspring);
        // Mutation
        switch(mOptions.mutationType) {
        case UNIFORM_MUTATION:
            uniformMutate(offspring, mOptions.mutationUniformPerc);
            break;
        case GAUSSIAN_MUTATION:
            gaussianMutate(offspring);
            break;
        }

        mNewPopulation[k] = offspring;
        ++k;
    }

    int interval = ceil((float)(mNp-elitismIndex)/(float)nThread);

    thread_params localThreadParam[nThread];

    for (int i = 0; i < nThread; ++i) {
        localThreadParam[i].startIndex = elitismIndex + interval*i;
        localThreadParam[i].endIndex = min(localThreadParam[i].startIndex + interval, (int)mNp);
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

    if(mOptions.selectionSorting) {
        partial_sort(mNewPopulation.begin(), mNewPopulation.begin()+elitismIndex, mNewPopulation.end());
    }

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        for(int i=0; i<mNx; i++) {
            mSigma[i] = mOptions.mutationGaussianScale*(1.0 - mOptions.mutationGaussianShrink*((float)mGeneration/(float)mMaxGenerations));
        }
    }

    mPopulation = mNewPopulation;

    evalMinFitness();
    evalMaxFitness();

    mGeneration++;
}
