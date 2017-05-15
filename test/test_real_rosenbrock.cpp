#include "testcommon.h"

double rosenbrock(RealGenotype &g, void *par) {
	double dx1 = g.gene[0]*g.gene[0]-g.gene[1];
	double dx2 = 1.0 - g.gene[0];
	return 100*dx1*dx1+dx2*dx2;
}

void test_rosenbrock(GAOptions opt, GAResults &results) {
	/*
	cout << "================================"<<endl;
	cout << "        Rosenbrock benchmark" << endl;
	cout << "================================"<<endl; 
	*/
	float LB[] = {-48.0, -48.0},
				UB[] = { 48.0,  48.0};

	RealGenotype expMin(2);
	expMin.gene[0] = 1.0;
	expMin.gene[1] = 1.0;

	strcpy(results.name, "Rosenbrock");
	results.maxIter = 50000;
	results.Np = 200;
	
	RealGen ga(results.Np, 2, LB, UB, opt);
	ga.setFitnessFunction(rosenbrock, NULL);

	testRealGen(ga, results.maxIter, 1e-4, expMin, results);
}