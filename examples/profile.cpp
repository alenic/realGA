#include <iostream>
#include <time.h>
#include "realga.h"
#include "fitnessfunction.h"

#define N_SAMPLE 1000

using namespace std;

class QuadraticFitness : public FitnessFunction {
public:
    int mN;

    QuadraticFitness(int n) {
        mN = n;
    }

    float eval(const RealChromosome &g) {
        float solution_i = 2.0f;
        float cost = 0.0;
        float dx;
        for(int i=0; i<mN; i++) {
            dx = (g.gene[i] - solution_i);
            cost += dx*dx;
            solution_i = -solution_i;
        }
        return cost;
    }
};


int main(int argc,  char** argv) {
    int N = stoi(argv[1]);
    int nIter = 1000;
    cout << "N " << N << endl;
    cout << "num iterations: " << nIter << endl;

    vector<float> LB(N);  // Lower bound of genes
    vector<float> UB(N);  // Upper bound of genes

    for(int i=0; i<N; i++) {
        LB[i] = -5;
        UB[i] = 5;
    }

    QuadraticFitness *myFitnessFunction = new QuadraticFitness(N);
    RealGAOptions options;
    options.setChromosomeSize(N);
    options.setPopulationSize(100*N);
    options.setBounds(LB, UB);
    options.setMutationType("gaussian");
    options.setMutationRate(0.1);
    options.setMutationGaussianPerc(1.0 / (float(nIter)*0.8), 0.001);
    options.setSelectionType("roulette");
    //options.setSelectionTournamentProbability(0.8);
    options.setVerbose(0);


    RealGA ga;
    ga.init(options, myFitnessFunction, false);

    // Init population with uniform random
    ga.popInitRandUniform();
    // Evolve the population for 100 times
    for (int i=0; i<nIter; i++) {
        ga.evolve();
    }
    // get the best score function (the minimum)
    RealChromosome best = ga.getBestChromosome();
    // Print results
    cout << "Best solution: "<< best.toString() << endl;
    cout << "Best Fitness value: " << best.fitness << endl;

    delete myFitnessFunction;
    return 0;
}

