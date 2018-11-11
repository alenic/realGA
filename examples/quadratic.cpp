#include <iostream>
#include <time.h>
#include "realgen.h"

#define N_SAMPLE 1000

using namespace std;

double myFitnessFunction(RealGenotype &g, void *par) {
  double f = 0.0;
  double dx1 = g.gene[0]-2.5,
         dx2 = g.gene[1]-2.5;
  return dx1*dx1+dx2*dx2;
}

int main(int argc,  char** argv) {
  float LB[] = {-5.0, -5.0};  // Lower bound of genes
  float UB[] = { 5.0,  5.0};  // Upper bound of genes
  RealGenOptions options;
  options.setGenesNumber(2);
  options.setPopulationSize(200);
  options.setBounds(LB, UB);
  options.setVerbose(true);
  options.setFitnessFunction(myFitnessFunction, NULL);
  options.setMaxGenerations(200);
  options.setMutationType("gaussian");
  options.setMutationRate(0.05);
  options.setMutationGaussianScaleShrink(1, 1);
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  RealGen ga(options);

  // Init population with uniform random
  ga.initRandom();
  // Evolve the population for 100 times
  for (int i=0; i<100; i++) {
    ga.evolve();
    cout << "Diversity " << ga.getDiversity() << endl;
  }
  // get the best score function (the minimum)
  RealGenotype best = ga.getBestChromosome();
  // Print results
  cout << ga.populationToString(); // print all the population
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;

  return 0;
}

