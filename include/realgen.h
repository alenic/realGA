/*

realgen.h : genetic algorithm core

author: A. Nicolosi
realGen project: https://github.com/alenic/realGen

*/
#ifndef REALGEN_H
#define REALGEN_H
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include "realgenotype.h"
#include "stat.h"

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=0, TOURNMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=0, FIXED_CROSSOVER, SINGLE_POINT_CROSSOVER, TWO_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=0, GAUSSIAN_MUTATION} MutationType;

struct SelectionOpt {
	SelectionType type;
	int tournmentP;
	bool sorting;
	float elitismFactor;
};

struct CrossoverOpt {
	CrossoverType type;
	int fixedIndex;
};

struct MutationOpt {
	MutationType type;
	float uniformPerc;
	float mutationRate;
	float gaussianScale;
	float gaussianShrink;
}; 

struct GAOptions {
	SelectionOpt selection;
	CrossoverOpt crossover;
	MutationOpt mutation;
	bool verbose;

	GAOptions() {
		selection.type = ROULETTE_WHEEL_SELECTION;
		selection.tournmentP = 4;
		selection.sorting = true;
		selection.elitismFactor = 0.1;

		crossover.type = UNIFORM_CROSSOVER;
		crossover.fixedIndex = 0;

		mutation.type = UNIFORM_MUTATION;
		mutation.uniformPerc = 0.25;

		mutation.mutationRate = 0.1;

		mutation.gaussianScale = 1.0;
		mutation.gaussianShrink = 1.0;

		verbose = false;
	}
};

class RealGen {
protected:
	vector <RealGenotype> population;
	vector <RealGenotype> newPopulation;
	size_t Np;
	size_t Nx;
	float *LB;
	float *UB;
	GAOptions options;
	double (*fitnessFcn)(RealGenotype &, void *);
	void *fitnessPar;
	int generation;
	Stat stat;
	int maxGenerations;  // Maximum number of generations (default: 100*number of variables)
	double sumFitnessR; // Only for roulette wheel selection
	int *tourIndex; // Only for tournment selection
	float *sigma;

	double minFitness;
	int iminFitness;
	double maxFitness;
	int imaxFitness;
	double meanFitness;
	void evalMinFitness();
	void evalMaxFitness();

public:
	RealGen(int np, int nx, float *lb, float *ub);
	RealGen(int np, int nx, float *lb, float *ub, GAOptions);
	~RealGen();
	// Setter
	void setFitnessFunction(double (*f)(RealGenotype &, void *), void *);
	void setSorting(bool);
	void setMutationRate(float);
	void setElitismFactor(float);
	void setPopulationSize(int);
	void setOptions(GAOptions opt);
	void setMaxGenerations(int);
	void setSeed(unsigned int);

	// getter
	RealGenotype getBestChromosome();
	int getGeneration();
	double evalFitness(RealGenotype &);
	double getMeanFitness();
	double getMinFitness();
	string populationToString();
	double diversity();
	void checkOptions();

	//  ================= Initialization =================================
	void initRandom();
	void evalPopulationFitness();
	void initMutate(vector<float> &gene, float sigma);
	// ================= Selection =================================
	void rouletteWheelSelection(int &index1, int &index2);
	void sumFitnessRoulette();
	void rouletteWheel(int &index, float stop);
	void tournmentSelection(int p, int &index1, int &index2);
	void tournmentSelect(int p, int &index);
	//==================================== Crossover ==================
	void crossoverUniform(int index1, int index2, RealGenotype &c);
	void crossoverFixed(int index1, int index2, RealGenotype &c, int n);
	//===================================== Mutation ======================
	void uniformMutate(RealGenotype &g, float perc);
	void gaussianLocalMutate(RealGenotype &);
	// ====================================================================
	virtual void evolve();
};

#endif // REALGEN_H