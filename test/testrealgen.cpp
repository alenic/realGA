#include "testcommon.h"

void testRealGen(RealGen &ga, int maxIter, float eps, RealGenotype &expMin, bool &converged, int &iter, double &bestFitness, float &exTime) {
	int generation = 0;
	RealGenotype *best;

	converged = false;
	ga.checkOptions();
	ga.initRandom();
	ga.setMaxGenerations(maxIter);
	clock_t startTime = clock(), endTime;
	for (int i=0; i<maxIter; i++) {
		ga.evolve();
		generation++;
		best = ga.getBestChromosome();

		if(best->distanceTo(expMin) < 1e-4 && !converged) {
			iter = generation;
			converged = true;
		}
	}
	endTime = clock();
	if(!converged)
		iter = maxIter;

	bestFitness = best->fitness;
	exTime = float( endTime - startTime ) / (float)CLOCKS_PER_SEC;
	
	cout << "Fitness"<< best->toString() << " = " << best->fitness << endl;
	cout << "Convergence: " << iter << endl;
	cout << "Time for " << maxIter << " iterations: " << exTime << " s." << endl;
	
}
