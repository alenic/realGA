/*

stat.h : genetic algorithm statistics

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

*/
#ifndef STAT_H_
#define STAT_H_
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>

using namespace std;

class Stat {
public:
    static float uniformRand();
    static float uniformRand(float lb, float ub);
    static float gaussianRand(float m, float s);
    static int uniformIndex(int N);
    static void setSeed(unsigned int seed);
};

#endif // STAT_H_
