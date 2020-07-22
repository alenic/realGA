#include <iostream>
#include "realgen.h"
#include "fitnessfunction.h"

using namespace std;

/* Fitness function: x1^2 + x2^2 */
class QuadraticFitness : public FitnessFunction {
public:
  QuadraticFitness() {}

  double eval(const RealGenotype &g) {
    return g.gene[0]*g.gene[0] + g.gene[1]*g.gene[1];
  }
};


int main(int argc,  char** argv) {
  vector<float> LB = {-5.0, -5.0};  // Lower bound of genes
  vector<float> UB = { 5.0,  5.0};  // Upper bound of genes
  QuadraticFitness *myFitnessFunction = new QuadraticFitness();
  RealGenOptions options;
  options.setSolutionSize(2);
  options.setPopulationSize(50);
  options.setBounds(LB, UB);
  options.setVerbose(true);
  options.setFitnessFunction(myFitnessFunction);
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  RealGen ga(options);
  
  // Init population with uniform random
  ga.initRandom();
  // Evolve the population for 100 times
  for (int i=0; i<100; i++) {
    ga.evolve();
  }
  // get the best score function (the minimum)
  RealGenotype best = ga.getBestChromosome();
  // Print results
  cout << ga.populationToString(); // print all the population
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;
  
  delete myFitnessFunction;
  return 0;
}

