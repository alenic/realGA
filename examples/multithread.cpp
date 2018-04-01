#include <iostream>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "realgenmultithread.h"

#define N_SAMPLE 5000

using namespace std;

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp () {
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

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

void optimize(unsigned int seed, unsigned int nThreads) {
  // Define RealGen(Population size, number of genes in a chromosome, LB, UB)
  GAOptions opt;
  opt.mutation.type = GAUSSIAN_MUTATION;
  opt.mutation.mutationRate = 0.05;
  opt.mutation.gaussianScale = 1;
  opt.mutation.gaussianShrink = 1;

  RealGen *ga;
  
  if (nThreads==1) {
    ga = new RealGen(203, 10, LB, UB, opt);
  } else {
    ga = new RealGenMultithread(203, 10, LB, UB, opt, nThreads);
  }
  
  ga->setFitnessFunction(myFitnessFunction, NULL);
  ga->setMaxGenerations(1500);
  ga->setSeed(seed);

  // Init population with uniform random genes between LB and UB
  ga->initRandom();
  // Evolve the population for 1500 times
  for (int i=0; i<1500; i++) {
    ga->evolve();
    RealGenotype best = ga->getBestChromosome();
  }
  // get the best score function (the minimum)
  RealGenotype best = ga->getBestChromosome();
  // Print results
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;
  delete ga;
}


int main(int argc,  char** argv) {
  timestamp_t t0, t1;
  unsigned int seed = 45;

  // Used for generate A matrix and b vector
  generateData();
  
  cout << "Single optimization...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 1);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;
  
  cout << "Multithread optimization (2 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 2);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;
  
  cout << "Multithread optimization (4 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 4);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;

  cout << "Multithread optimization (8 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 8);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;

  return 0;
}

