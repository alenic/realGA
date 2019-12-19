/*

fitnessfunction.h: base class for fitness function

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

*/
#ifndef FITNESSFUNCTION_H
#define FITNESSFUNCTION_H

#include "realgenotype.h"

class FitnessFunction {
  public:
  virtual double eval(const RealGenotype &x) = 0;
};


#endif // FITNESSFUNCTION_H