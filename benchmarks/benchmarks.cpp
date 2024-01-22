#include "benchmarks.h"


void benchmarkRealGen(realGA &ga, int maxIter, float eps, GAResults &results, int numExperiments) {
    vector<GAResults> resultsVector(numExperiments);
    int generation;

    for(int k=0; k<numExperiments; k++) {
        generation = 0;

        ga.popInitRandUniform();
        ga.restart();

        resultsVector[k].converged = 0;
        clock_t startTime = clock();
        for(int i=0; i<maxIter; i++) {
            ga.evolve();
            generation++;
            if(ga.getBestChromosome().fitness < eps || i==(maxIter-1)) {
                if(i!=(maxIter-1)) resultsVector[k].converged = 1;
                
                resultsVector[k].iter = (float)generation;
                resultsVector[k].convergedTime = float( clock() - startTime ) / (float)CLOCKS_PER_SEC;
                resultsVector[k].best = ga.getBestChromosome();
                resultsVector[k].bestFitness = resultsVector[k].best.fitness;
                break;
            }
        }
    }

    // Compute statistics
    results.converged = 0;
    results.iter = 0;
    results.convergedTime = 0;

    float minFitness=MAXFLOAT;
    for(int i=0; i<numExperiments; i++) {
        if(resultsVector[i].bestFitness < minFitness) {
            results.bestFitness = resultsVector[i].bestFitness;
            results.best = resultsVector[i].best;
            minFitness = results.bestFitness;
        }
        results.converged += resultsVector[i].converged;
        results.iter += resultsVector[i].iter;
        results.convergedTime += resultsVector[i].convergedTime;
        
    }
    results.iter /= numExperiments;
    results.convergedTime /= numExperiments;
    results.converged /= numExperiments;
    results.printResults();
}

void benchmark_all(RealGAOptions &opt) {
    GAResults results;
    printf("%-15s %-12s %-12s %-12s %-12s\n", "Test", "Converged", "Iterations", "Time", "Min Fitness");

    benchmark_sphere(opt, results);
    benchmark_rosenbrock(opt, results);
    benchmark_flatSurface(opt, results);
    benchmark_foxholes(opt, results);
}

int main() {
    GAResults results[5];
    RealGAOptions opt;
    int chromosomeSize = 2;
    
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
    for(int i=0; i<5; i++) {
        benchmark_fake(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;
    }

    printf("Selection type = \"tournament\"\n");
    opt.setMutationType("uniform");
    opt.setSelectionType("tournament");
    chromosomeSize = 2;
    for(int i=0; i<5; i++) {
        benchmark_fake(opt, results[i], chromosomeSize, 1000);
        chromosomeSize *= 2;    }

    return 0;
}