#include "realgen.h"
#include <iomanip>

#define BENCH_NUM_EXPERIMENTS 10

struct GAResults {
    string name;
    int Np;
    float maxIter;
    float iter;
    float converged;
    float eps;
    float bestFitness;
    float maxTime;
    float convergedTime;
    RealChromosome best;

    void printResults() {
        printf("%-15s %-12f %-12f %-12f %-12f\n", name.c_str(), converged, iter, convergedTime, bestFitness);
    }
};

void benchmarkRealGen(RealGen &ga, int maxIter, float eps, GAResults &results, int numExperiments);

// Benchmark Problems
void benchmark_all(RealGenOptions &opt);
void benchmark_fake(RealGenOptions opt, GAResults &results, int chromosomeSize, int populationSize);
// Problems
void benchmark_sphere(RealGenOptions opt, GAResults &results);
void benchmark_rosenbrock(RealGenOptions opt, GAResults &results);
void benchmark_flatSurface(RealGenOptions opt, GAResults &results);
void benchmark_foxholes(RealGenOptions opt, GAResults &results);