#include "testcommon.h"

void coutColor(const char * s, TextColor color) {
    switch(color) {
    case TEXT_RED:
        cout << "\033[31m" << s << "\033[0m";
        break;
    case TEXT_GREEN:
        cout << "\033[32m" << s << "\033[0m";
        break;
    case TEXT_YELLOW:
        cout << "\033[33m" << s << "\033[0m";
        break;
    }
}

void testRealGen(RealGen &ga, int maxIter, float eps, GAResults &results) {
    int generation = 0;

    results.converged = false;
    //ga.checkOptions();
    ga.initRandom();
    ga.setMaxGenerations(maxIter);
    clock_t startTime = clock(), endTime;
    for (int i=0; i<maxIter; i++) {
        ga.evolve();
        generation++;
        results.best = ga.getBestChromosome();

        if(results.best.fitness < eps && !results.converged) {
            results.iter = generation;
            results.exTime = float( clock() - startTime ) / (float)CLOCKS_PER_SEC;
            results.bestFitness = results.best.fitness;
            results.best = ga.getBestChromosome();
            results.converged = true;
        }
    }
    endTime = clock();

    results.maxTime = float( endTime - startTime ) / (float)CLOCKS_PER_SEC;

    if(!results.converged) {
        results.iter = maxIter;
        results.exTime = results.maxTime;
        results.bestFitness = results.best.fitness;
        results.best = ga.getBestChromosome();
    }
}
