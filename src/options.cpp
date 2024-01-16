/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "options.h"


RealGenOptions::RealGenOptions() {
    populationSize = 100;

    maxGenerations = 100 * populationSize;

    selectionType = ROULETTE_WHEEL_SELECTION;
    selectionTournamentSize = 16;
    selectionTournamentProbability = 1.0;

    selectionElitismFactor = 0.1;

    crossoverType = UNIFORM_CROSSOVER;
    crossoverindexA = 0;
    crossoverindexB = 0;

    mutationType = UNIFORM_MUTATION;
    mutationUniformPerc = 0.25;         // percentage of UB-LB

    mutationRate = 0.1;
    

    mutationGaussianPercDelta = 0.01;    // 100 iterations decay to 0.01
    mutationGaussianPercMin= 0.001;     // 1000 iterations

    verbose = false;
    seed = 42;

}

RealGenOptions::~RealGenOptions() {

}

void RealGenOptions::setPopulationSize(int value) {
    populationSize = value;
}

void RealGenOptions::setChromosomeSize(int value) {
    chromosomeSize = value;
}

void RealGenOptions::setLowerBounds(const vector<float> &lb) {
    lowerBounds = lb;
}

void RealGenOptions::setUpperBounds(const vector<float> &ub) {
    upperBounds = ub;
}

void RealGenOptions::setBounds(const vector<float> &lb, const vector<float> &ub) {
    lowerBounds = lb;
    upperBounds = ub;
}

void RealGenOptions::setMaxGenerations(int value) {
    maxGenerations = value;
}

void RealGenOptions::setVerbose(bool value) {
    verbose = value;
}


void RealGenOptions::setSeed(bool seedValue) {
    seed = seedValue;
}

void RealGenOptions::setSelectionType(string value) {
    if (value == "roulette")
        selectionType = ROULETTE_WHEEL_SELECTION;
    else if (value == "tournament")
        selectionType = TOURNAMENT_SELECTION;
    else
        cerr << "setSelectionType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setSelectionTournamentSize(int value) {
    selectionTournamentSize = value;
}

void RealGenOptions::setSelectionTournamentProbability(float value) {
    selectionTournamentSize = value;
}


void RealGenOptions::setElitismFactor(float factor) {
    if(factor >= 0.0 && factor <= 1.0) {
        selectionElitismFactor = factor;
    } else {
        cerr << "ERROR: options.selection.elitismFactor must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }
}

void RealGenOptions::setMutationType(string value) {
    if (value == "uniform")
        mutationType = UNIFORM_MUTATION;
    else if (value == "gaussian")
        mutationType = GAUSSIAN_MUTATION;
    else
        cerr << "setMutationType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setUniformMutationPercentage(float value) {
    mutationUniformPerc = value;
}

void RealGenOptions::setMutationRate(float rate) {
    if(rate >= 0.0 && rate <= 1.0) {
        mutationRate = rate;
    } else {
        cerr << "ERROR: options.mutation.mutationRate must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }
}

void RealGenOptions::setMutationGaussianPerc(float percDelta, float percMin) {
    mutationGaussianPercDelta = percDelta;
    mutationGaussianPercMin = percMin;
}

void RealGenOptions::setCrossoverType(string value) {
    if (value == "uniform")
        crossoverType = UNIFORM_CROSSOVER;
    else if (value == "single_point")
        crossoverType = SINGLE_POINT_CROSSOVER;
    else if (value == "two_point")
        crossoverType = TWO_POINT_CROSSOVER;
    else
        cerr << "setCrossoverType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setSinglePointCrossoverIndex(int value) {
    crossoverindexA = value;
}

void RealGenOptions::setTwoPointCrossoverIndexes(int i1, int i2) {
    crossoverindexA = i1;
    crossoverindexB = i2;
}

