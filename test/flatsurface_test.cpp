#include "testcommon.h"
#include "fitnessfunction.h"

class FlatSurfaceFitness : public FitnessFunction {
public:
    FlatSurfaceFitness() {}

    float eval(const RealChromosome &g) {
        float fitness = 0.0;
        for(int i=0; i<g.gene.size(); i++) {
            fitness += fabs(floor(g.gene[i]));
        }
        return fitness;
    }
};


void flatSurface_problem(RealGenOptions opt, GAResults &results) {
    vector<float> LB = { -5.12, -5.12, -5.12, -5.12, -5.12 };
    vector<float> UB = { 5.12,  5.12,  5.12,  5.12,  5.12};

    FlatSurfaceFitness *myFitnessFunction = new FlatSurfaceFitness();
    results.name = "Flat surface";
    results.maxIter = 5000;
    results.Np = 200;

    opt.setPopulationSize(results.Np);
    opt.setChromosomeSize(5);
    opt.setBounds(LB, UB);

    RealGen ga;
    ga.init(opt, myFitnessFunction, false);

    testRealGen(ga, results.maxIter, 1, results);

    delete myFitnessFunction;
}
