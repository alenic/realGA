#include <iostream>
#include <time.h>
#include "realgen.h"

#define N_SAMPLE 1000

using namespace std;

float LB[] = {-5.0, -5.0};  // Lower bound of genes
float UB[] = { 5.0,  5.0};  // Upper bound of genes

double myFitnessFunction(RealGenotype &g, void *par) {
  double f = 0.0;
  double dx1 = g.gene[0]-2.5,
         dx2 = g.gene[1]-2.5;
  return dx1*dx1+dx2*dx2;
}


int main(int argc,  char** argv) {
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  GAOptions opt;
  opt.mutation.type = GAUSSIAN_MUTATION;
  opt.mutation.mutationRate = 0.05;
  opt.mutation.gaussianScale = 1;
  opt.mutation.gaussianShrink = 1;
  
  RealGen ga(200, 2, LB, UB, opt);
  ga.setFitnessFunction(myFitnessFunction, NULL);
  ga.setMaxGenerations(200);
  // Init population with uniform random
  ga.initRandom();
  // Evolve the population for 100 times
  for (int i=0; i<100; i++) {
    ga.evolve();
    cout << "Diversity " << ga.diversity() << endl;
  }
  // get the best score function (the minimum)
  RealGenotype best = ga.getBestChromosome();
  // Print results
  cout << ga.populationToString(); // print all the population
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;

  return 0;
}

