#include "testcommon.h"

double flatSurfaceFitness(RealGenotype &g, void *par) {
  double fitness = 0.0;
  for(size_t i=0; i<g.gene.size(); i++) {
    fitness += fabs(floor(g.gene[i]));
  }
  return fitness;
}

void bench3_flatSurface(RealGenOptions opt, GAResults &results) {
  float LB[] = {-5.12, -5.12, -5.12, -5.12, -5.12},
      UB[] = { 5.12,  5.12,  5.12,  5.12,  5.12};

  strcpy(results.name, "Flat surface");
  results.maxIter = 5000;
  results.Np = 200;

  opt.setPopulationSize(results.Np);
  opt.setGenesNumber(5);
  opt.setBounds(LB, UB);
  opt.setFitnessFunction(flatSurfaceFitness, NULL);  

  RealGen ga(opt);

  testRealGen(ga, results.maxIter, 1, results);
}
