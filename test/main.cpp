#include "testcommon.h"
#define NTESTFUNC 2
#include <stdio.h>

int main() {
	bool converged[NTESTFUNC];
	int iter[NTESTFUNC];
	double bestFitness[NTESTFUNC];
	float exTime[NTESTFUNC];
	GAOptions opt;

	srand(time(NULL));
	test_uniformDist();
	test_gaussianDist();

	test_real_sphere(opt, converged[0], iter[0], bestFitness[0], exTime[0]);
	test_rosenbrock(opt, converged[1], iter[1], bestFitness[1], exTime[1]);

	printf("%-20s%-10s%-10s%-10s%-10s\n", "Test", "Converged", "Fitness", "iter", "maxTime");
	printf("%-20s%-10d%-10f%-10d%-10f\n", "Sphere", converged[0], bestFitness[0], iter[0], exTime[0]);

	opt.mutation.type = GAUSSIAN_MUTATION;
	test_real_sphere(opt, converged[0], iter[0], bestFitness[0], exTime[0]);
	test_rosenbrock(opt, converged[1], iter[1], bestFitness[1], exTime[1]);
	return 0;
}