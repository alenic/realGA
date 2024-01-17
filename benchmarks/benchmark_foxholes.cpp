#include "benchmarks.h"

class FoxholesFitness : public FitnessFunction {
public:
    FoxholesFitness(float *A) {mA = A; }

    float eval(const RealChromosome &g) {
        // min at (-31.97833,-31.97833)
        float sum1 = 0.0;
        for(int i=0; i<25; i++) {
            float dx1 = g.gene[0]-mA[i];
            float dx2 = g.gene[1]-mA[i+25];
            sum1 += 1.0/(i + dx1*dx1 + dx2*dx2);
        }
        return 1/500.0 + 1.0/sum1;
    }

private:
    float *mA;
};

void benchmark_foxholes(RealGenOptions opt, GAResults &results) {
    vector<float> LB = { -65536, -65536 };
    vector<float> UB = { 65536,  65536};
    float A[] = {-32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32,
                 -32, -32, -32, -32, -32, -16, -16, -16, -16, -16, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16};
    FoxholesFitness *myFitnessFunction = new FoxholesFitness(A);

    results.name = "Foxholes";
    results.maxIter = 1000;
    results.Np = 200;

    opt.setPopulationSize(results.Np);
    opt.setChromosomeSize(2);
    opt.setBounds(LB, UB);

    RealGen ga;
    ga.init(opt, myFitnessFunction, false);

    benchmarkRealGen(ga, results.maxIter, 1e-1, results, BENCH_NUM_EXPERIMENTS);

    delete myFitnessFunction;
}
