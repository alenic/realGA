#include <iostream>
#include "realga.h"
#include "fitnessfunction.h"

using namespace std;

/* Fitness function: x1^2 + x2^2 */
class QuadraticFitness : public FitnessFunction {
public:
    QuadraticFitness() {}

    float eval(const RealChromosome &g) {
        return g.gene[0]*g.gene[0] + g.gene[1]*g.gene[1];
    }
};


int main(int argc,  char** argv) {
    vector<float> LB = {-5.0, -5.0};  // Lower bound of genes
    vector<float> UB = { 5.0,  5.0};  // Upper bound of genes
    QuadraticFitness *myFitnessFunction = new QuadraticFitness();
    RealGAOptions options;
    options.setChromosomeSize(2);
    options.setPopulationSize(50);
    options.setBounds(LB, UB);
    options.setVerbose(1);

    // Init Genetic Algorithm with options, fitness function and keepState=false
    RealGA ga;
    ga.init(options, myFitnessFunction, false);

    // Init population with uniform random between LB and UB
    ga.popInitRandUniform();

    // Evolve the population for 100 times
    for (int i=0; i<100; i++) {
        ga.evolve();
    }
    // get the best score function (the minimum)
    RealChromosome best = ga.getBestChromosome();
    // Print results
    cout << "Best solution: "<< best.toString() << endl;
    cout << "Best Fitness value = " << best.fitness << endl;

    delete myFitnessFunction;
    return 0;
}

