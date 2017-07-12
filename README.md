# realGen
Fast implementation of Genetic Algorithm only for real genotype, useful for nonlinear constrained and unconstrained optimization problems.

## INSTALL
```
$ git clone https://github.com/alenic/realGen.git
$ cd realGen
$ make
```
# Quick start

First of all you have to define a fitness function to minimize, and just pass it as function pointer to the RealGen class with the method setFitnessFunction:

```c++
#include <iostream>
#include "realgen.h"

using namespace std;

/* Fitness function: x1^2 + x2^2 */
double myFitnessFunction(RealGenotype &g, void *par) {
  return g.gene[0]*g.gene[0] + g.gene[1]*g.gene[1];
}


int main(int argc,  char** argv) {
  float LB[] = {-5.0, -5.0};  // Lower bound of genes
  float UB[] = { 5.0,  5.0};  // Upper bound of genes
  
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  RealGen ga(50, 2, LB, UB);
  ga.setFitnessFunction(myFitnessFunction, NULL);
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
  
  return 0;
}

  ```
  
