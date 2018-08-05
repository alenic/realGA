# Real Gen
Fast implementation of Genetic Algorithm for nonlinear constrained and unconstrained optimization problems.
This library implements only real genotype, it means that a **solution** of the problem must be an N-dimensional array of **float** numbers (x_1, x_2, ... , x_n)

where every gene x_i can be bounded by

* LB_i <= x_i <= UB_i

for i = 1,...,N

Real Gen is available for

* Windows 7/10
* Linux

## INSTALL

Download the repository with the command

```
git clone https://github.com/alenic/realGen.git
```

Use CMake to compile it.


### Windows

```
cd realGen
mkdir build
cd build
cmake ..
MSBuild.exe .\realGen.sln /p:Configuration={Release|Debug}
```

If you want to use Win64 compiler use CMake as follows

```
cmake -G "Visual Studio 14 2015 Win64" ..
```

### Linux

```
cd realGen
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE={Release|Debug} ..
make
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

  RealGenOptions options;
  options.setGenesNumber(2);      // Number of genes in a chromosome
  options.setPopulationSize(50);  // Number of chromosomes in the population
  options.setBounds(LB, UB);      // LB and UB are mandatory
  options.setVerbose(true);       // If Verbose is true, manny prints will appear
  options.setFitnessFunction(myFitnessFunction, NULL);
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
  
  return 0;
}

  ```
  
  It can be compiled and linked statically with the command (after INSTALL step)
  ```
  g++ -std=c++11 -I$(REALGEN_INCLUDE_FOLDERS) -o readme.bin readme.cpp $(REALGEN_LIB_FOLDER)/librealgen.a
  ```
  

