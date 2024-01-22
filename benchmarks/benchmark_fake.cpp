#include "benchmarks.h"

class Benchmark : public FitnessFunction {
public:
    Benchmark() {}

    float eval(const RealChromosome &g) {
        return Stat::randUniform();
    }
};

void benchmark_fake(RealGAOptions opt, GAResults &results, int chromosomeSize, int populationSize) {
    vector<float> LB(chromosomeSize);
    vector<float> UB(chromosomeSize);

    LB.assign(chromosomeSize, 0);
    UB.assign(chromosomeSize, 1);

    Benchmark *myFitnessFunction = new Benchmark();
    results.name = "Bench_"+to_string(chromosomeSize)+"_"+to_string(populationSize);
    results.maxIter = 500;
    results.Np = populationSize;

    opt.setPopulationSize(populationSize);
    opt.setChromosomeSize(chromosomeSize);
    opt.setBounds(LB, UB);

    realGA ga;
    ga.init(opt, myFitnessFunction, false);

    benchmarkRealGen(ga, results.maxIter, 0.0f, results, 1);

    delete myFitnessFunction;
}