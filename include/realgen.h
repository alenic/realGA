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
#include "realgenoptions.h"
#include "fitnessfunction.h"

using namespace std;

class RealGen {
protected:
  vector <RealGenotype> population;
  vector <RealGenotype> newPopulation;
  GAOptions options;
  size_t Np;
  size_t Nx;

  FitnessFunction *fitnessFcn;
  void *fitnessPar;
  int generation;

  int maxGenerations;  // Maximum number of generations (default: 100*number of variables)
  double sumFitnessR; // Only for roulette wheel selection
  int *tourIndex; // Only for tournament selection
  float *sigma;

  double minFitness;
  int iminFitness;
  double maxFitness;
  int imaxFitness;
  void evalMinFitness();
  void evalMaxFitness();

public:
  RealGen(RealGenOptions &opt);
  ~RealGen();
  // Setter
  void setFitnessFunction(FitnessFunction *f);
  void setSorting(bool);
  void setMutationRate(float);
  void setElitismFactor(float);
  void setPopulationSize(int);
  void setOptions(RealGenOptions &opt);
  void setMaxGenerations(int);
  void setSeed(unsigned int);

  // getter
  RealGenotype getBestChromosome();
  int getGeneration();
  double evalFitness(const RealGenotype &);
  double getMeanFitness();
  double getMinFitness();
  string populationToString();
  double getDiversity();
  void checkOptions();
  // For debug
  bool checkChromosome(RealGenotype &chromosome);
  void checkPopulation();

  //  ================= Initialization =================================
  void initRandom();
  void evalPopulationFitness();
  void initMutate(vector<float> &gene, float sigma);
  // ================= Selection =================================
  void rouletteWheelSelection(int &index1, int &index2);
  void sumFitnessRoulette();
  void rouletteWheel(int &index, float stop);
  void tournamentSelection(int p, int &index1, int &index2);
  void tournamentSelect(int p, int &index);
  //==================================== Crossover ==================
  void crossoverUniform(int index1, int index2, RealGenotype &c);
  void crossoverFixed(int index1, int index2, RealGenotype &c, int n);
  //===================================== Mutation ======================
  void uniformMutate(RealGenotype &g, float perc);
  void gaussianMutate(RealGenotype &);
  // ====================================================================
  virtual void evolve();
};

#endif // REALGEN_H
