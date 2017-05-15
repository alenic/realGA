#include "testcommon.h"

double real_sphere(RealGenotype &g, void *par) {
	double sse = 0.0;
	for(size_t i=0; i<g.gene.size(); i++) {
		double dx = g.gene[i]/5.12;
		sse += dx*dx;
	}
	return sse;
}


void test_real_sphere(GAOptions opt, GAResults &results) {
	/*
	cout << "================================"<<endl;
	cout << "        Sphere benchmark" << endl;
	cout << "================================"<<endl; 
	*/
	float LB[] = {-5.12, -5.12, -5.12, -5.12},
				UB[] = { 5.12,  5.12,  5.12,  5.12};
	RealGenotype expMin(4);
	expMin.gene[0] = 0.0;
	expMin.gene[1] = 0.0;
	expMin.gene[2] = 0.0;
	expMin.gene[3] = 0.0;

	strcpy(results.name, "Sphere");
	results.maxIter = 50000;
	results.Np = 50;
	
	RealGen ga(results.Np, 4, LB, UB, opt);
	ga.setFitnessFunction(real_sphere, NULL);

	testRealGen(ga, results.maxIter, 1e-4, expMin, results);
}