/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        FitnessFunction class is an abstract class
        that compute the value of the fitness function
        given a chromosome (solution x). You have
        to inherit this class to define your own fitness
        function. 
---------------------------------------------
*/
#ifndef REALGA_FITNESS_FUNCTION_H
#define REALGA_FITNESS_FUNCTION_H

#include "chromosome.h"

class FitnessFunction {
public:
    virtual float eval(const RealChromosome &x) = 0;
    virtual ~FitnessFunction() {};
};


#endif // REALGA_FITNESS_FUNCTION_H
