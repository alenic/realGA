/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic

*/
#ifndef FITNESSFUNCTION_H
#define FITNESSFUNCTION_H

#include "realchromosome.h"

class FitnessFunction {
public:
    virtual float eval(const RealChromosome &x) = 0;
    virtual ~FitnessFunction() {};
};


#endif // FITNESSFUNCTION_H
