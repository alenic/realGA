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
  for (int k = tp->startIndex; k < (tp->startIndex+tp->neval) && (k < tp->ga->Np); ++k) {
    ga->newPopulation[k].fitness = ga->evalFitness(ga->newPopulation[k]);
  }
  pthread_exit(NULL);
}
#endif



void RealGenMultithread::evolve() {
  RealGenotype offspring(Nx);
  offspring.setBounds(options.lowerBounds, options.upperBounds);

  int index1=0, index2=1;
  int elitismIndex = (int)(options.selection.elitismFactor*Np);
  size_t k=0;

  if(options.selection.type == ROULETTE_WHEEL_SELECTION) {
    sumFitnessRoulette();
  }

  if (options.selection.sorting) {
    while (k < elitismIndex) {
      newPopulation[k] = population[k];
      ++k;
    }
  } else {
    // Keep only the best one
    newPopulation[k] = population[iminFitness];
    ++k;
  }

  while(k < Np) {
    // Selection
    switch(options.selection.type) {
      case ROULETTE_WHEEL_SELECTION:
        rouletteWheelSelection(index1, index2);
      break;
      case TOURNAMENT_SELECTION:
        tournamentSelection(options.selection.tournamentP, index1, index2);
      break;
    }
    
    // Crossover
    crossoverUniform(index1, index2, offspring);
    // Mutation
    switch(options.mutation.type) {
      case UNIFORM_MUTATION:
        uniformMutate(offspring, options.mutation.uniformPerc);
      break;
      case GAUSSIAN_MUTATION:
        gaussianMutate(offspring);
      break;
    }
    
    newPopulation[k] = offspring;
    ++k;
  }
  
  int interval = floor((float)(Np-elitismIndex)/(float)nThread);

  thread_params *localThreadParam = new thread_params[nThread];
  for (int i = 0; i < nThread; ++i) {
    localThreadParam[i].startIndex = elitismIndex + interval*i;
    localThreadParam[i].neval = interval;
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

  delete[] localThreadParam;

  if(options.selection.sorting) {
    partial_sort(newPopulation.begin(), newPopulation.begin()+elitismIndex, newPopulation.end());
  }

  if(options.mutation.type == GAUSSIAN_MUTATION) {
    for(int i=0; i<Nx; i++) {
      sigma[i] = options.mutation.gaussianScale*(1.0 - options.mutation.gaussianShrink*((float)generation/(float)maxGenerations))*0.1;
    }
  }

  population = newPopulation;

  evalMinFitness();
  evalMaxFitness();

  generation++;
}
