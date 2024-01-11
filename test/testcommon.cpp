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
    cout << endl;
}


void printDistribution(vector<float> &x) {
    int nstars = 400;
    int bins = 20;
    int *freq = (int *)calloc(bins, sizeof(int));

    float minV = *std::min_element(x.begin(), x.end());
    float maxV = *std::max_element(x.begin(), x.end());
    float delta = maxV - minV;

    for (int i=0; i<x.size(); ++i) {
        float value = bins*(x[i] - minV) / delta;
        ++freq[int(value)];
    }

    cout << "Histogram" << endl;
    for (int i=0; i<bins; ++i) {
        cout << ": ";
        cout << string(freq[i] * nstars/x.size(), '*') << endl;
    }
    cout << endl;
    free(freq);
}

void testRealGen(RealGen &ga, int maxIter, float eps, GAResults &results) {
    int generation = 0;

    results.converged = false;
    //ga.checkOptions();
    ga.popInitRandUniform();
    clock_t startTime = clock(), endTime;
    for (int i=0; i<maxIter; i++) {
        ga.evolve();
        generation++;
        results.best = ga.getBestChromosome();

        if(results.best.fitness < eps && !results.converged) {
            results.iter = generation;
            results.convergedTime = float( clock() - startTime ) / (float)CLOCKS_PER_SEC;
            results.bestFitness = results.best.fitness;
            results.best = ga.getBestChromosome();
            results.converged = true;
        }
    }
    endTime = clock();

    results.maxTime = float( endTime - startTime ) / (float)CLOCKS_PER_SEC;

    if(!results.converged) {
        results.iter = maxIter;
        results.convergedTime = results.maxTime;
        results.bestFitness = results.best.fitness;
        results.best = ga.getBestChromosome();
    }
}
