#include "testcommon.h"

double rosenbrockFitness(RealGenotype &g, void *par) {
	double dx1 = g.gene[0]*g.gene[0]-g.gene[1];
	double dx2 = 1.0 - g.gene[0];
	return 100.0*dx1*dx1+dx2*dx2;
}

void bench2_rosenbrock(GAOptions opt, GAResults &results) {
	float LB[] = {-2.048, -2.048},
		  UB[] = { 2.048,  2.048};

	strcpy(results.name, "Rosenbrock");
	results.maxIter = 5000;
	results.Np = 200;
	
	RealGen ga(results.Np, 2, LB, UB, opt);
	ga.setFitnessFunction(rosenbrockFitness, NULL);

	testRealGen(ga, results.maxIter, 1e-4, results);
}
