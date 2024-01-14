#include "realgen.h"

struct GAResults {
    string name;
    int Np;
    int maxIter;
    int iter;
    bool converged;
    float eps;
    float bestFitness;
    float maxTime;
    float convergedTime;
    RealChromosome best;
};

void benchmarkRealGen(RealGen &ga, int maxIter, float eps, GAResults &results);

// Benchmark Problems
void benchmark_all(RealGenOptions &opt);
void benchmark_fake(RealGenOptions opt, GAResults &results, int chromosomeSize, int populationSize);
// Problems
void benchmark_sphere(RealGenOptions opt, GAResults &results);
void benchmark_rosenbrock(RealGenOptions opt, GAResults &results);
void benchmark_flatSurface(RealGenOptions opt, GAResults &results);
void benchmark_foxholes(RealGenOptions opt, GAResults &results);