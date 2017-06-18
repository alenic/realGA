#include "testcommon.h"
#define NTESTFUNC 2
#include <stdio.h>


int main() {
	GAResults results[NTESTFUNC];
	GAOptions opt;

	srand(time(NULL));
	test_uniformDist();
	test_gaussianDist(5.0, 2.0);

	test_real_sphere(opt, results[0]);
	test_rosenbrock(opt, results[1]);

	printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "exTime", "maxTime", "Fitness");
	
	for(int i=0; i<NTESTFUNC; i++) {
		printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name, results[i].converged, results[i].iter, results[i].maxIter, results[i].exTime, results[i].maxTime, results[i].bestFitness);
	}

	opt.mutation.type = GAUSSIAN_MUTATION;
	test_real_sphere(opt, results[0]);
	test_rosenbrock(opt, results[1]);

	printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "maxIter", "iter", "exTime", "maxTime", "Fitness");

	for(int i=0; i<NTESTFUNC; i++) {
		printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name, results[i].converged, results[i].iter, results[i].maxIter, results[i].exTime, results[i].maxTime, results[i].bestFitness);
	}
	return 0;
}