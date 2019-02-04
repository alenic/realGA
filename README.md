# Real Gen
Fast implementation of Genetic Algorithm for nonlinear constrained optimization problems.<br>
The **Fitness** function is a function, defined by the user, that must be minimized. The Fitness, usually is a nonlinear Loss Function, that depends on a solution of the problem, this 'solution' is called **chromosome**.<br>
A chromosome, is composed by n components called **genes**.<br>
Every genes, are simply real numbers, and then, a chromosome can be view as a vector of real numbers. In this view, the Fitness is just a nonlinear multivariate function.

If x is the cromosome then

![genotype](docs/img/x_vec.png)

x_i are the genes (for i =1,...,n). <br>

Every genes x_i are bounded by two real value: 

* LB_i <= x_i <= UB_i,  for i = 1,...,n.

A **genetic algorithm** is a probabilistic method for find the minimum of the Fitness function, and it solves the problem

![genotype](docs/img/minimum.png)


Real Gen is available for

* Windows 7/10
* Linux

See the <a href='docs/index.md'>documentations</a> for more informations.

## Install

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
  options.setVerbose(true);       // If Verbose is true, many prints will appear
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
  
Compile with
  ```
  g++ -std=c++11 -I$(REALGEN_INCLUDE_FOLDERS) -o readme.bin readme.cpp $(REALGEN_LIB_FOLDER)/librealgen.a
  ```
  

