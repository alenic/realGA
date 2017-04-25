#include "testcommon.h"

double rosenbrock(RealGenotype &g, void *par) {
	double dx1 = g.gene[0]*g.gene[0]-g.gene[1];
	double dx2 = 1.0 - g.gene[0];
	return 100*dx1*dx1+dx2*dx2;
}


void test_rosenbrock() {
	srand(time(NULL));
	float LB[] = {-2048.0, -2048.0},
		  UB[] = { 2048.0,  2048.0};

	RealGenotype exp_min(2);
	exp_min.gene[0] = 1.0;
	exp_min.gene[1] = 1.0;

	RealGen ga(200, 2, LB, UB);
	
	// Options
	ga.setFitnessFunction(rosenbrock, NULL);
	//ga.checkOptions();

	ga.initRandom();

	int generation = 0;
	int convergence;
	bool converged = false;
	RealGenotype *best;
	int N = 50000;
	clock_t startTime = clock(), endTime;
	for (int i=0; i<N; i++) {
		ga.evolve();
		generation++;
		best = ga.getBestChromosome();

		if(best->distanceTo(exp_min) < 1e-4 && !converged) {
			convergence = generation;
			converged = true;
		}
	}
	endTime = clock();
	if(!converged)
		convergence = N;

	cout << "================================"<<endl;
	cout << "        Rosenbrock benchmark" << endl;
	cout << "================================"<<endl; 
	cout << ga.populationToString();
	cout << "Convergence generation: " << convergence << endl;
	cout << "Solution: "<< best->toString() << endl;
	cout << "Fitness = " << best->fitness << endl;
	cout << "Time for " << N << " iterations: " << float( endTime - startTime ) / (float)CLOCKS_PER_SEC<< " s." << endl;
}