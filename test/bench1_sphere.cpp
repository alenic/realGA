#include "testcommon.h"

double sphereFitness(RealGenotype &g, void *par) {
  double sse = 0.0;
  for(size_t i=0; i<g.gene.size(); i++) {
    double dx = g.gene[i]/5.12;
    sse += dx*dx;
  }
  return sse;
}


void bench1_sphere(RealGenOptions opt, GAResults &results) {
  float LB[] = {-5.12, -5.12, -5.12, -5.12},
      UB[] = { 5.12,  5.12,  5.12,  5.12};

  strcpy(results.name, "Sphere");
  results.maxIter = 5000;
  results.Np = 50;
  
  opt.setPopulationSize(results.Np);
  opt.setGenesNumber(4);
  opt.setBounds(LB, UB);
  opt.setFitnessFunction(sphereFitness, NULL);
  RealGen ga(opt);

  testRealGen(ga, results.maxIter, 1e-4, results);
}
