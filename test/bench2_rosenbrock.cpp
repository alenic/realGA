#include "testcommon.h"
#include "fitnessfunction.h"

class RosenbrockFitness : public FitnessFunction {
public:
  RosenbrockFitness() {}

  double eval(const RealGenotype &g) {
    double dx1 = g.gene[0]*g.gene[0]-g.gene[1];
    double dx2 = 1.0 - g.gene[0];
    return 100.0*dx1*dx1+dx2*dx2;
    }
};

void bench2_rosenbrock(RealGenOptions opt, GAResults &results) {
  vector<float> LB = { -2.048, -2.048 };
  vector<float> UB = { 2.048,  2.048};

  RosenbrockFitness *myFitnessFunction = new RosenbrockFitness();
  strcpy(results.name, "Rosenbrock");
  results.maxIter = 5000;
  results.Np = 200;
  
  opt.setPopulationSize(results.Np);
  opt.setSolutionSize(2);
  opt.setBounds(LB, UB);
  opt.setFitnessFunction(myFitnessFunction);
  RealGen ga(opt);

  testRealGen(ga, results.maxIter, 1e-4, results);

  delete myFitnessFunction;
}
