#include "testcommon.h"
#define MAX_RESULTS_BUFFER 16

int main() {
    GAResults results[MAX_RESULTS_BUFFER];
    RealGenOptions opt;

    StatTest statUT;
    RealChromosomeTest realChromosomeUT;
    SelectionTests selectionUT;

    cout << "==================== Statistical Test ============================" << endl;
    statUT.test_uniformDist();
    statUT.test_gaussianDist(5.0, 2.0);

    cout << "==================== RealChromosome Test ============================" << endl;
    realChromosomeUT.test_randUniform();
    realChromosomeUT.test_uniformMutate();

    cout << "==================== Selection Test ============================" << endl;
    selectionUT.test_RWsearchIndexBinarySearch();
    selectionUT.test_RWselect();
    
    cout << "==================== GA Test ============================" << endl;

    opt.setMutationType("uniform");
    opt.setSelectionType("roulette");
    printf("Mutation type=uniform - Selection Type=roulette\n");
    testProblems(opt);

    opt.setMutationType("uniform");
    opt.setSelectionType("tournament");
    printf("Mutation type=uniform - Selection Type=tournament\n");
    testProblems(opt);

    opt.setMutationType("gaussian");
    opt.setSelectionType("roulette");
    opt.setMutationGaussianPerc(0.025, 0.001);
    printf("Mutation type=gaussian - Selection Type=roulette\n");
    testProblems(opt);

    opt.setMutationType("gaussian");
    opt.setSelectionType("tournament");
    opt.setMutationGaussianPerc(0.025, 0.001);
    printf("Mutation type=gaussian - Selection Type=tournament\n");
    testProblems(opt);
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
