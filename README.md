# Real Gen
Fast implementation of Genetic Algorithm for nonlinear constrained optimization problems.

* $F(x)$ represents the **fitness function**, a non-linear function defined by the user. The objective is to identify the *minimum value* of this function.
* $x = (x_1, x_2, ..., x_n)$ is the input of the fitness function and is called **chromosome**
* $x_i$ is the i-th component of the chromosome $x$ is called **gene**
* $L=(l_1, l_2, ... , l_n)$ and $U=(u_1, u_2, ... , u_n)$ are the **lower** and **upper** bounds respectively of the chromosome, specifying the range within which the gene values must fall: $l_i \le x_i \le u_i$
* A **genetic algorithm** is a probabilistic optimization method employed to discover the minimum of a fitness function. It effectively addresses the following problem:

$$x^* = \arg \min_{L \le x \le U} F(x)$$

# Install

Download the repository with the command

```
git clone https://github.com/alenic/realGen.git
```

Create the forlder build for cmake:

```
cd realGen
mkdir build
cd build
```

Use CMake to build the library 

## Linux

```
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Windows (Visual Studio 14)

```
cmake ..
MSBuild.exe .\realGen.sln /p:Configuration=Release
```

If you want to use Win64 compiler use the command:

```
cmake -G "Visual Studio 14 2015 Win64" ..
```

It will generates the static library librealgen.a and the share library librealgen_shared.so into the folder build/Libs

# Test
To test you can execute the test_main program:

```
realGen/build > ./Tests/test_main
```

# Compile
Use the following command to compile a .cpp file that use librealgen.a:
```
g++ -std=c++11 -I$(REALGEN_INCLUDE_FOLDER) -o readme.bin readme.cpp $(REALGEN_LIB_FOLDER)/librealgen.a
```

* REALGEN_INCLUDE_FOLDERS is the folder of include files, (the **include** folder)
* REALGEN_LIB_FOLDER is the folder where the librealgen.a file was generated 

# Quick start

You can find the following code example inside examples/readme.cpp

```c++
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
    options.setChromosomeSize(2);
    options.setPopulationSize(50);
    options.setBounds(LB, UB);

    // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
    RealGen ga(options);
    ga.setFitnessFunction(myFitnessFunction);
    ga.setVerbose(true);

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

  ```
  

