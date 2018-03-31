#include <iostream>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "realgenmultithread.h"

#define N_SAMPLE 1000

using namespace std;

float LB[] = {-5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0};  // Lower bound of genes
float UB[] = { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0};  // Upper bound of genes

float A[N_SAMPLE][10];
float b[N_SAMPLE];


double myFitnessFunction(RealGenotype &g, void *par) {
  double f = 0.0;
  float Ag[N_SAMPLE];
  
  // Compute A*g
  for(int i=0; i<N_SAMPLE; ++i) {
    Ag[i] = 0;
    for(int j=0; j<10; j++) {
      Ag[i] += A[i][j]*g.gene[j];
    }
  }

  // Compute A*g-b
  float M = -1;
  for(int i=0; i<N_SAMPLE; ++i) {
    Ag[i] = Ag[i] - b[i];
    if(fabs(Ag[i]) > M) {
      M = fabs(Ag[i]);
    }
  }

  // Compute norm2
  for(int i=0; i<N_SAMPLE; ++i) {
    float uAg = Ag[i]/M;
    f += uAg*uAg;
  }

  return fabs(M)*sqrt(f);
}

void generateData() {
  Stat stat;
  float par[] = {1.0, -1.0, 0.5, -0.5, -2.5, 1.0, 2.5, -1.5, -2.0, 4.0};

  for(int i=0; i<N_SAMPLE; ++i) {
    b[i] = 0;
    for(int j=0; j<10; ++j) {
      if (j % 2 == 0) {
        A[i][j] = cos(j*((float)i/N_SAMPLE));
      } else {
        A[i][j] = sin(j*((float)i/N_SAMPLE));
      }
    }
  }

  for(int i=0; i<N_SAMPLE; ++i) {
    b[i] = 0;
    for(int j=0; j<10; ++j) {
      b[i] += par[j]*A[i][j];
    }
  }
}

void optimize(unsigned int seed) {
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  GAOptions opt;
  opt.mutation.type = GAUSSIAN_MUTATION;
  opt.mutation.mutationRate = 0.05;
  opt.mutation.gaussianScale = 1;
  opt.mutation.gaussianShrink = 1;
  
  RealGen ga(203, 10, LB, UB, opt);
  ga.setFitnessFunction(myFitnessFunction, NULL);
  ga.setMaxGenerations(1500);
  ga.setSeed(seed);
  // Init population with uniform random
  
  ga.initRandom();
  // Evolve the population for 100 times
  for (int i=0; i<1500; i++) {
    ga.evolve();
    RealGenotype best = ga.getBestChromosome();
  }
  // get the best score function (the minimum)
  RealGenotype best = ga.getBestChromosome();
  // Print results
  //cout << ga.populationToString(); // print all the population
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;
}


void optimizeMultithread(unsigned int seed) {
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  GAOptions opt;
  opt.mutation.type = GAUSSIAN_MUTATION;
  opt.mutation.mutationRate = 0.05;
  opt.mutation.gaussianScale = 1;
  opt.mutation.gaussianShrink = 1;
  
  RealGenMultithread ga(203, 10, LB, UB, opt, 4);
  ga.setFitnessFunction(myFitnessFunction, NULL);
  ga.setMaxGenerations(1500);
  ga.setSeed(seed);
  // Init population with uniform random
  ga.initRandom();
  // Evolve the population for 100 times
  for (int i=0; i<1500; i++) {
    ga.evolve();
    RealGenotype best = ga.getBestChromosome();
  }
  // get the best score function (the minimum)
  RealGenotype best = ga.getBestChromosome();
  // Print results
  //cout << ga.populationToString(); // print all the population
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;
}

int main(int argc,  char** argv) {
  struct timeval start_time;
  struct timeval end_time;
  srand(1);
  generateData();
  
  //gettimeofday(&start_time, NULL);
  cout << "Single optimization...."<< endl;
  optimize(3);
  optimize(4);
  //gettimeofday(&end_time, NULL); 
  //cout << "Eapsed Time: " << end_time.tv_usec-start_time.tv_usec << endl;
  
  
  cout << "Multithread optimization...."<< endl;
  gettimeofday(&start_time, NULL);
  optimizeMultithread(3);
  gettimeofday(&end_time, NULL);
  cout << "Eapsed Time: " << end_time.tv_usec-start_time.tv_usec << endl;
  
  return 0;
}

