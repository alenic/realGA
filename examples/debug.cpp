#include <iostream>
#include <time.h>
#include "realga.h"
#include "fitnessfunction.h"

#define N_SAMPLE 1000

using namespace std;

class QuadraticFitness : public FitnessFunction {
public:
    float eval(const RealChromosome &g) {
        float dx1 = g.gene[0]-2.5,
                dx2 = g.gene[1]-2.5;
        return dx1*dx1+dx2*dx2;
    }
};


int main(int argc,  char** argv) {
    vector<float> LB = {-5.0, -5.0};  // Lower bound of genes
    vector<float> UB = { 5.0,  5.0};  // Upper bound of genes
    QuadraticFitness *myFitnessFunction = new QuadraticFitness();
    RealGAOptions options;
    options.setChromosomeSize(2);
    options.setPopulationSize(200);
    options.setBounds(LB, UB);
    options.setMutationType("gaussian");
    options.setMutationRate(0.1);
    options.setMutationGaussianPerc(1.0/10.0, 0.001);
    options.setSelectionType("roulette");
    options.setSelectionTournamentProbability(0.8);
    options.setVerbose("soft");
    // Define realGA(Population size, number of genes in a chromosome, LB, UB)
    realGA ga;
    ga.init(options, myFitnessFunction, false);


    // Init population with uniform random
    ga.popInitRandUniform();

    RealChromosome best = ga.getBestChromosome();
    cout << ga.populationToString(); // print all the population
    cout << "Best solution: "<< best.toString() << endl;
    cout << "Best Fitness value = " << best.fitness << endl;

    // Evolve the population for 100 times
    for (int i=0; i<1000; i++) {
        ga.evolve();
        // Print results
        RealChromosome best = ga.getBestChromosome();
        cout << ga.populationToString(); // print all the population
        cout << "Best solution: "<< best.toString() << endl;
        cout << "Best Fitness value = " << best.fitness << endl;
    }

    delete myFitnessFunction;
    return 0;
}

