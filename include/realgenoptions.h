#ifndef REALGENOPTIONS_H
#define REALGENOPTIONS_H
#include <string>
#include <iostream>
#include "realgenotype.h"
#include "fitnessfunction.h"

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=0, TOURNAMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=0, SINGLE_POINT_CROSSOVER, TWO_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=0, GAUSSIAN_MUTATION} MutationType;

struct SelectionOpt {
  SelectionType type;
  int tournamentP;
  bool sorting;
  float elitismFactor;
};

struct CrossoverOpt {
  CrossoverType type;
  unsigned int index1;
  unsigned int index2;
};

struct MutationOpt {
  MutationType type;
  float uniformPerc;
  float mutationRate;
  float gaussianScale;
  float gaussianShrink;
};

/*
  Backend option struct
*/
struct GAOptions {
  SelectionOpt selection;
  CrossoverOpt crossover;
  MutationOpt mutation;
  bool verbose;
  size_t genesNumber;
  size_t populationSize;
  float *lowerBounds;
  float *upperBounds;
  FitnessFunction *fitnessFcnPtr;
  void *fitnessPar;
  size_t maxGenerations;
  GAOptions();
};

/*
  Class used for option settings
*/
class RealGenOptions {
public:
  RealGenOptions();
  ~RealGenOptions();

  void setVerbose(bool value);

  void setPopulationSize(size_t np);
  void setGenesNumber(size_t nx);
  void setLowerBounds(float *lb);
  void setUpperBounds(float *ub);
  void setBounds(float *lb, float *ub);
  void setFitnessFunction(FitnessFunction *f);
  void setMaxGenerations(size_t value);

  void setSelectionType(string value);
  void setSelectionTournamentP(size_t value);
  void setSorting(bool value);
  void setElitismFactor(float value);

  void setMutationType(string value);
  void setUniformMutationPercentage(float value);
  void setMutationRate(float value);
  void setMutationGaussianScaleShrink(float scale, float shrink);

  void setCrossoverType(string value);
  void setSinglePointCrossoverIndex(size_t value);
  void setTwoPointCrossoverIndexes(size_t i1, size_t i2);

  GAOptions getOptions(void);
private:
  GAOptions opt;
};


#endif