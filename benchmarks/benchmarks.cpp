#include "benchmarks.h"


void benchmarkRealGen(RealGen &ga, int maxIter, float eps, GAResults &results) {
    int generation = 0;

    results.converged = false;
    //ga.checkOptions();
    ga.popInitRandUniform();
    clock_t startTime = clock(), endTime;
    for (int i=0; i<maxIter; i++) {
        ga.evolve();
        generation++;
        results.best = ga.getBestChromosome();

        if(results.best.fitness < eps && !results.converged) {
            results.iter = generation;
            results.convergedTime = float( clock() - startTime ) / (float)CLOCKS_PER_SEC;
            results.bestFitness = results.best.fitness;
            results.best = ga.getBestChromosome();
            results.converged = true;
        }
    }
    endTime = clock();

    results.maxTime = float( endTime - startTime ) / (float)CLOCKS_PER_SEC;

    if(!results.converged) {
        results.iter = maxIter;
        results.convergedTime = results.maxTime;
        results.bestFitness = results.best.fitness;
        results.best = ga.getBestChromosome();
    }
}

void benchmark_all(RealGenOptions &opt) {
    GAResults results[4];

    benchmark_sphere(opt, results[0]);
    benchmark_rosenbrock(opt, results[1]);
    benchmark_flatSurface(opt, results[2]);
    benchmark_foxholes(opt, results[3]);
    
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");

    for(int i=0; i<4; i++) {
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f ", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
        cout << results[i].best.toString() << endl;
    }
}

int main() {
    GAResults results[5];
    RealGenOptions opt;

    cout << "==================== GA Test ============================" << endl;

    opt.setMutationType("uniform");
    opt.setSelectionType("roulette");
    printf("Mutation type=uniform - Selection Type=roulette\n");
    benchmark_all(opt);

    opt.setMutationType("uniform");
    opt.setSelectionType("tournament");
    printf("Mutation type=uniform - Selection Type=tournament\n");
    benchmark_all(opt);

    opt.setMutationType("gaussian");
    opt.setSelectionType("roulette");
    opt.setMutationGaussianPerc(0.025, 0.001);
    printf("Mutation type=gaussian - Selection Type=roulette\n");
    benchmark_all(opt);

    opt.setMutationType("gaussian");
    opt.setSelectionType("tournament");
    opt.setMutationGaussianPerc(0.025, 0.001);
    printf("Mutation type=gaussian - Selection Type=tournament\n");
    benchmark_all(opt);

    cout << "==================== GA Fake Benchmark ============================" << endl;
    printf("Selection type = \"roulette\"\n");
    opt.setMutationType("uniform");
    opt.setSelectionType("roulette");
    int chromosomeSize = 2;
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");
    for(int i=0; i<5; i++) {
        benchmark_fake(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
    }

    printf("Selection type = \"tournament\"\n");
    opt.setSelectionType("tournament");
    chromosomeSize = 2;
    printf("%-15s%-12s%-12s%-12s%-12s%-12s%-12s\n", "Test", "Converged", "iter", "maxIter", "convTime", "maxTime", "Fitness");
    for(int i=0; i<5; i++) {
        benchmark_fake(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;
        printf("%-15s%-12d%-12d%-12d%-12.6f%-12.6f%-12f\n", results[i].name.c_str(), results[i].converged, results[i].iter, results[i].maxIter, results[i].convergedTime, results[i].maxTime, results[i].bestFitness);
    }

    return 0;
}