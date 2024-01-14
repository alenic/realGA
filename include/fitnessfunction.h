/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic

*/
#ifndef REALGEN_FITNESS_FUNCTION_H
#define REALGEN_FITNESS_FUNCTION_H

#include "chromosome.h"

class FitnessFunction {
public:
    virtual float eval(const RealChromosome &x) = 0;
    virtual ~FitnessFunction() {};
};


#endif // REALGEN_FITNESS_FUNCTION_H
