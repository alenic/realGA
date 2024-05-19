/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        RealChromosome is the solution x of the
        optimization problem of type real (float)
---------------------------------------------
*/
#ifndef REALGA_CHROMOSOME_H
#define REALGA_CHROMOSOME_H
#include "stat.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>


using namespace std;

class RealChromosome {
public:
    vector<float> gene;
    float fitness;

    RealChromosome();
    RealChromosome(int n);
    RealChromosome(const RealChromosome &c);
    ~RealChromosome();

    string toString();

    void randUniform(vector<float> &lb, vector<float> &ub);
    void randUniform(int i, vector<float> &lb, vector<float> &ub);
    void randGaussian(float mean, float sigma,  vector<float> &lb, vector<float> &ub);
    void randGaussian(int i, float mean, float sigma, vector<float> &lb, vector<float> &ub);

    float distanceTo(RealChromosome &);

    RealChromosome & operator= ( const RealChromosome &c );
    bool operator<(const RealChromosome &c) const;
    bool operator==(const RealChromosome &other) const;
};

#endif // REALGA_CHROMOSOME_H
