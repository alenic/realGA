#include "realgenmultithread.h"

// Constructors
RealGenMultithread::RealGenMultithread(int np, int nx, float *lb, float *ub, unsigned int nt) : RealGen(np, nx, lb, ub) {
	nThread = nt;
	localThread = new pthread_t[nThread]; 
}

RealGenMultithread::RealGenMultithread(int np, int nx, float *lb, float *ub, GAOptions opt, unsigned int nt) : RealGen(np, nx, lb, ub, opt) {
	nThread = nt;
	localThread = new pthread_t[nThread]; 
}

RealGenMultithread::~RealGenMultithread() {
	delete []localThread;
}

void *RealGenMultithread::evaluatePopulationThread(void *params) {
	struct thread_params *tp;
	tp = (struct thread_params *) params;
	RealGenMultithread * ga = tp->ga;
	for(int k=tp->startIndex; k<tp->startIndex+tp->neval; ++k) {
		ga->newPopulation[k].fitness = ga->evalFitness(ga->newPopulation[k]);
	}
	pthread_exit(NULL);
}

void RealGenMultithread::evolve() {
	RealGenotype offspring(Nx);

	int index1, index2;
	size_t k=0;

	if(options.selection.type == ROULETTE_WHEEL_SELECTION) {
		sumFitnessRoulette();
	}

	if(options.selection.sorting) {
		while(k< options.selection.elitismFactor*Np) {
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
			case TOURNMENT_SELECTION:
				tournmentSelection(options.selection.tournmentP, index1, index2);
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
				gaussianLocalMutate(offspring);
			break;
		}
		
		newPopulation[k] = offspring;
		++k;
	}
	
	int interval = floor((float)Np/(float)nThread);
	int rc;
	thread_params *localThreadParam = new thread_params[nThread];
	for(int i=0; i<nThread; ++i) {
		localThreadParam[i].startIndex = interval*i;
		if(i == nThread-1) {
			localThreadParam[i].neval = interval + (Np % nThread);
		} else {
			localThreadParam[i].neval = interval;
		}
		localThreadParam[i].ga = this;
		rc = pthread_create(&localThread[i], NULL, RealGenMultithread::evaluatePopulationThread, (void *)&localThreadParam[i]);
		if (rc) {
			cerr << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
	}

	for(int i=0; i<nThread; ++i) {
		pthread_join(localThread[i], NULL);
	}




	if(options.selection.sorting) {
		partial_sort(newPopulation.begin(), newPopulation.begin()+int(options.selection.elitismFactor*Np), newPopulation.end());
	}

	if(options.mutation.type == GAUSSIAN_MUTATION) {
		for(int i=0; i<Nx; i++) {
			sigma[i] = options.mutation.gaussianScale*(1.0 - options.mutation.gaussianShrink*((float)generation/(float)maxGenerations))*0.1;
		}
	}

	evalMinFitness();
	evalMaxFitness();
	meanFitness = getMeanFitness();
	newPopulation.swap(population);
	generation++;
	delete[] localThreadParam;
}
