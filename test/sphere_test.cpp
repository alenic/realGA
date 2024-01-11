#include "testcommon.h"
#include "fitnessfunction.h"


class SphereFitness : public FitnessFunction {
public:
    SphereFitness() {}

    float eval(const RealChromosome &g) {
        float sse = 0.0;
        for(int i=0; i<g.gene.size(); i++) {
            float dx = g.gene[i]/5.12;
            sse += dx*dx;
        }
        return sse;
    }
};



void sphere_problem(RealGenOptions opt, GAResults &results) {
    vector<float> LB = { -5.12, -5.12, -5.12, -5.12 };
    vector<float> UB = { 5.12,  5.12,  5.12,  5.12};

    SphereFitness *myFitnessFunction = new SphereFitness();

    results.name = "Sphere";
    results.maxIter = 5000;
    results.Np = 50;

    opt.setPopulationSize(results.Np);
    opt.setChromosomeSize(4);
    opt.setBounds(LB, UB);

    RealGen ga;
    ga.init(opt, myFitnessFunction, false);
    
    testRealGen(ga, results.maxIter, 1e-4, results);

    delete myFitnessFunction;
}
