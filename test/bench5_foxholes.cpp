#include "testcommon.h"

double foxholesFitness(RealGenotype &g, void *par) {
  // min at (-31.97833,-31.97833)
  double sum1 = 0.0;
  float *A = (float *)par;
  for(int i=0; i<25; i++) {
    double dx1 = g.gene[0]-A[i];
    double dx2 = g.gene[1]-A[i+25];
    sum1 += 1.0/(i + dx1*dx1 + dx2*dx2);
  }
  return 1/500.0 + 1.0/sum1;
}

void bench5_foxholes(RealGenOptions opt, GAResults &results) {
  float LB[] = {-65536, -65536},
      UB[] = { 65536,  65536};

  strcpy(results.name, "Foxholes");
  results.maxIter = 5000;
  results.Np = 200;

  opt.setPopulationSize(results.Np);
  opt.setGenesNumber(2);
  opt.setBounds(LB, UB);
  opt.setFitnessFunction(foxholesFitness, NULL);


  RealGen ga(opt);
  float A[50] = {-32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32, -32, 16, 0, 16, 32, 
           -32, -32, -32, -32, -32, -16, -16, -16, -16, -16, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16};
  ga.setFitnessFunction(foxholesFitness, A);

  testRealGen(ga, results.maxIter, 1e-2, results);
}
