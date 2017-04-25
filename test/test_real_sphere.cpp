#include "testcommon.h"

double real_sphere(RealGenotype &g, void *par) {
	double sse = 0.0;
	for(size_t i=0; i<g.gene.size(); i++) {
		double dx = g.gene[i]/5.12;
		sse += dx*dx;
	}
	return sse;
}


void test_real_sphere() {
	srand(time(NULL));
	float LB[] = {-5.12, -5.12, -5.12, -5.12},
		  UB[] = { 5.12,  5.12,  5.12,  5.12};
	RealGenotype exp_min(4);
	exp_min.gene[0] = 0.0;
	exp_min.gene[1] = 0.0;
	exp_min.gene[2] = 0.0;
	exp_min.gene[3] = 0.0;

	GAOptions opt;
	opt.selection.type = TOURNMENT_SELECTION;
	RealGen ga(50, 4, LB, UB, opt);
	// Options
	ga.setFitnessFunction(real_sphere, NULL);
	ga.initRandom();

	int generation = 0;
	int convergence;
	bool converged = false;
	RealGenotype *best;
	int N = 10000;
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
	cout << "        Sphere benchmark" << endl;
	cout << "================================"<<endl; 
	//cout << ga.populationToString();
	cout << "Convergence generation: " << convergence << endl;
	cout << "Solution: "<< best->toString() << endl;
	cout << "Fitness = " << best->fitness << endl;
	cout << "Time for " << N << " iterations: " << float( endTime - startTime ) / (float)CLOCKS_PER_SEC<< " s." << endl;
}