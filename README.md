# realGen
Fast implementation of Genetic Algorithm only for real genotype, useful for nonlinear constrained and unconstrained optimization problems.
This library implements only real genotype, it means that a **solution** of the problem must be an N-dimensional array of **float** numbers:

| gene 1 | gene 2 |   ...  | gene N |
| ------ | ------ | ------ | ------ |

where every gene can be limited by a real lower and upper bound

LB_i <= gene_i <= UB_i

for _i=1,...,N_



## INSTALL
```
> git clone https://github.com/alenic/realGen.git
> cd relGen
> mkdir build
> cd build
```

Use CMake for compile:

### Windows

```
> cmake ..
> MSBuild.exe .\realGen.sln /p:Configuration={Release|Debug}
```

For Win64 use the command

```
> cmake -G "Visual Studio 14 2015 Win64" ..
```

### Linux
```
> cmake -DCMAKE_BUILD_TYPE={Release|Debug} ..
> make
```

# Quick start

You can find the following code example inside examples/readme.cpp

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
  
  It can be compiled and linked statically with the command (after INSTALL step)
  ```
  g++ -std=c++11 -I$(REALGEN_INCLUDE_FOLDERS) -o readme.bin readme.cpp $(REALGEN_LIB_FOLDER)/librealgen.a
  ```
  

