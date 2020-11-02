/*

realgenotype.h : real [0, 1] bounded genotype

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

*/
#ifndef REALGENOTYPE_H
#define REALGENOTYPE_H
#include "stat.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>


using namespace std;

class RealGenotype {
public:
    vector<float> gene;
    double fitness;

    vector<float> LB;
    vector<float> UB;

    RealGenotype();
    RealGenotype(int n);
    RealGenotype(const RealGenotype &c);
    ~RealGenotype();

    string toString();

    void setBounds(const vector<float> &lb, const vector<float> &ub);
    void uniformRandom();
    void uniformRandom(int);
    void uniformLocalRandom(int, float);
    void gaussianLocalRandom(int, float);
    double distanceTo(RealGenotype &);

    RealGenotype & operator= ( const RealGenotype &c );
    bool operator<(const RealGenotype &c) const;
    bool operator==(const RealGenotype &other) const;
};

#endif // REALGENOTYPE_H
