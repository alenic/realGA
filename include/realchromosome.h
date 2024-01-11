/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic

*/
#ifndef REALCHROMOSOME_H
#define REALCHROMOSOME_H
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

    vector<float> LB;
    vector<float> UB;

    RealChromosome();
    RealChromosome(int n);
    RealChromosome(const RealChromosome &c);
    ~RealChromosome();

    string toString();

    void setBounds(const vector<float> &lb, const vector<float> &ub);
    void randUniform();
    void randUniform(int i);
    void randUniformPerc(int i, float perc);
    void randGaussianPerc(int i, float perc);
    float distanceTo(RealChromosome &);

    RealChromosome & operator= ( const RealChromosome &c );
    bool operator<(const RealChromosome &c) const;
    bool operator==(const RealChromosome &other) const;
};

#endif // REALCHROMOSOME_H
