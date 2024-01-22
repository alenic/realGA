#include "realga.h"
#include <iomanip>

#define BENCH_NUM_EXPERIMENTS 100

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

void benchmarkRealGen(realGA &ga, int maxIter, float eps, GAResults &results, int numExperiments);

// Benchmark Problems
void benchmark_all(RealGAOptions &opt);
void benchmark_fake(RealGAOptions opt, GAResults &results, int chromosomeSize, int populationSize);
// Problems
void benchmark_sphere(RealGAOptions opt, GAResults &results);
void benchmark_rosenbrock(RealGAOptions opt, GAResults &results);
void benchmark_flatSurface(RealGAOptions opt, GAResults &results);
void benchmark_foxholes(RealGAOptions opt, GAResults &results);