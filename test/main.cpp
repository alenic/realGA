#include "testcommon.h"
#define MAX_RESULTS_BUFFER 16

int main() {
    GAResults results[MAX_RESULTS_BUFFER];
    RealGenOptions opt;

    StatTest statUT;
    RealChromosomeTest realChromosomeUT;

    cout << "==================== Stat Test ============================" << endl;
    statUT.test_uniformDist();
    statUT.test_gaussianDist(5.0, 2.0);

    cout << "==================== RealChromosome Test ============================" << endl;
    realChromosomeUT.test_randUniform();
    realChromosomeUT.test_randUniformPerc();

    cout << "==================== GA Test ============================" << endl;

    sphere_problem(opt, results[0]);
    rosenbrock_problem(opt, results[1]);
    flatSurface_problem(opt, results[2]);
    foxholes_problem(opt, results[3]);

    printf("Mutation type = Uniform\n");
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");

    for(int i=0; i<4; i++) {
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f ", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
        cout << results[i].best.toString() << endl;
    }

    opt.setMutationType("gaussian");
    opt.setMutationGaussianScaleShrink(1.0, 2.0);
    sphere_problem(opt, results[0]);
    rosenbrock_problem(opt, results[1]);
    flatSurface_problem(opt, results[2]);
    foxholes_problem(opt, results[3]);

    printf("Mutation type = Gaussian\n");
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");

    for(int i=0; i<4; i++) {
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f ", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
        cout << results[i].best.toString() << endl;
    }


    cout << "==================== GA Benchmark ============================" << endl;
    
    printf("Mutation type = Uniform\n");
    opt.setMutationType("uniform");
    int chromosomeSize = 2;
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");
    for(int i=0; i<5; i++) {
        benchmark(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
    }

    printf("Mutation type = Gaussian\n");
    opt.setMutationType("gaussian");
    chromosomeSize = 2;
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");
    for(int i=0; i<5; i++) {
        benchmark(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
    }

    return 0;
}
