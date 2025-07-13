#include <iostream>
#include <time.h>
#include <cmath>
#include "realga.h"
#include "fitnessfunction.h"

#define N_SAMPLE 1000

using namespace std;

class QuadraticFitness : public FitnessFunction
{
public:
    float eval(const RealChromosome &g)
    {
        float dx = (g.gene[0] - 1.0f),
              dy = (g.gene[1] - 1.0f),
              cx = cos(g.gene[0]),
              sy = sin(g.gene[1]);
        return 0.1 * dx * dx + 0.1 * dy * dy - cx * cx - sy * sy;
    }
};

int main(int argc, char **argv)
{
    vector<float> LB = {-5.0, -5.0}; // Lower bound of genes
    vector<float> UB = {5.0, 5.0};   // Upper bound of genes
    QuadraticFitness *myFitnessFunction = new QuadraticFitness();
    RealGAOptions options;
    options.setChromosomeSize(2);
    options.setPopulationSize(100);
    options.setElitismFactor(0.20);
    options.setBounds(LB, UB);
    options.setMutationType("gaussian");
    options.setMutationRate(0.1);
    options.setMutationGaussianPerc(0.01, 0.01);
    options.setSelectionType("roulette");
    options.setSelectionTournamentProbability(0.8);
    options.setVerbose("soft");
    // Define RealGA(Population size, number of genes in a chromosome, LB, UB)
    RealGA ga;
    ga.init(options, myFitnessFunction, false);

    // Init population with uniform random
    ga.popInitRandUniform();

    RealChromosome best = ga.getBestChromosome();
    cout << ga.populationToCSVString(); // print all the population
    ga.populationToCSV("debug_csv/pop_0.csv");
    cout << "Best solution: " << best.toString() << endl;
    cout << "Best Fitness value = " << best.fitness << endl;

    // Evolve the population for 100 times
    for (int i = 0; i < 200; i++)
    {
        ga.evolve();
        // Print results
        RealChromosome best = ga.getBestChromosome();
        cout << ga.populationToCSVString(); // print all the population

        cout << "Best solution: " << best.toString() << endl;
        cout << "Best Fitness value = " << best.fitness << endl;

        std::string filename = "debug_csv/pop_" + std::to_string(i) + ".csv";
        ga.populationToCSV(filename);
    }

    delete myFitnessFunction;
    return 0;
}
