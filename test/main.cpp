#include "testcommon.h"
#define NTESTFUNC 4

int main() {
	GAResults results[NTESTFUNC];
	GAOptions opt;

	srand(time(NULL));

	StatTest statUT;
	RealGenotypeTest realGenotypeUT;

	cout << "==================== Stat Unit Tests ============================" << endl;
	statUT.test_uniformDist();
	statUT.test_gaussianDist(5.0, 2.0);

	cout << "==================== RealGenotype Unit Tests ============================" << endl;
	realGenotypeUT.test_uniformRandom();
	realGenotypeUT.test_uniformLocalRandom();

	cout << "==================== Integration Tests ============================" << endl;

	bench1_sphere(opt, results[0]);
	bench2_rosenbrock(opt, results[1]);
	bench3_flatSurface(opt, results[2]);
	bench5_foxholes(opt, results[3]);

	printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "exTime", "maxTime", "Fitness");
	
	for(int i=0; i<NTESTFUNC; i++) {
		printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name, results[i].converged, results[i].iter, results[i].maxIter, results[i].exTime, results[i].maxTime, results[i].bestFitness);
	}

	opt.mutation.type = GAUSSIAN_MUTATION;
	bench1_sphere(opt, results[0]);
	bench2_rosenbrock(opt, results[1]);
	bench3_flatSurface(opt, results[2]);
	bench5_foxholes(opt, results[3]);

	printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "maxIter", "iter", "exTime", "maxTime", "Fitness");

	for(int i=0; i<NTESTFUNC; i++) {
		printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name, results[i].converged, results[i].iter, results[i].maxIter, results[i].exTime, results[i].maxTime, results[i].bestFitness);
	}
	return 0;
}