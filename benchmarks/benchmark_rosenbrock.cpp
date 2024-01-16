#include "benchmarks.h"

class RosenbrockFitness : public FitnessFunction {
public:
    RosenbrockFitness() {}

    float eval(const RealChromosome &g) {
        float dx1 = g.gene[0]*g.gene[0]-g.gene[1];
        float dx2 = 1.0 - g.gene[0];
        return 100.0*dx1*dx1+dx2*dx2;
    }
};

void benchmark_rosenbrock(RealGenOptions opt, GAResults &results) {
    vector<float> LB = { -2.048, -2.048 };
    vector<float> UB = { 2.048,  2.048};

    RosenbrockFitness *myFitnessFunction = new RosenbrockFitness();

    results.name = "Rosenbrock";
    results.maxIter = 5000;
    results.Np = 200;

    opt.setPopulationSize(results.Np);
    opt.setChromosomeSize(2);
    opt.setBounds(LB, UB);
    RealGen ga;
    ga.init(opt, myFitnessFunction, false);

    benchmarkRealGen(ga, results.maxIter, 1e-3, results);

    delete myFitnessFunction;
}
