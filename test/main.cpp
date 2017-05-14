#include "testcommon.h"
#define NTESTFUNC 2

int main() {
	bool converged[NTESTFUNC];
	int iter[NTESTFUNC];
	double bestFitness[NTESTFUNC];
	GAOptions opt;

	srand(time(NULL));
	test_uniformDist();
	test_gaussianDist();

	test_real_sphere(opt, converged[0], iter[0], bestFitness[0]);
	test_rosenbrock(opt, converged[1], iter[1], bestFitness[1]);

	opt.mutation.type = GAUSSIAN_MUTATION;
	test_real_sphere(opt, converged[0], iter[0], bestFitness[0]);
	test_rosenbrock(opt, converged[1], iter[1], bestFitness[1]);
	return 0;
}