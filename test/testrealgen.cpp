#include "testcommon.h"

void testRealGen(RealGen &ga, int maxIter, float eps, RealGenotype &expMin, GAResults &results) {
	int generation = 0;

	results.converged = false;
	//ga.checkOptions();
	ga.initRandom();
	ga.setMaxGenerations(maxIter);
	clock_t startTime = clock(), endTime;
	for (int i=0; i<maxIter; i++) {
		ga.evolve();
		generation++;
		results.best = ga.getBestChromosome();

		if(results.best->distanceTo(expMin) < 1e-4 && !results.converged) {
			results.iter = generation;
			results.converged = true;
			results.exTime = float( clock() - startTime ) / (float)CLOCKS_PER_SEC;
		}
	}
	endTime = clock();
	results.bestFitness = results.best->fitness;
	results.maxTime = float( endTime - startTime ) / (float)CLOCKS_PER_SEC;

	if(!results.converged) {
		results.iter = maxIter;
		results.exTime = results.maxTime;
	}

	/*
	cout << "Fitness"<< best->toString() << " = " << best->fitness << endl;
	cout << "Convergence: " << iter << endl;
	cout << "Time for " << maxIter << " iterations: " << exTime << " s." << endl;
	*/
}
