#ifndef REALGENOPTIONS_H
#define REALGENOPTIONS_H
#include <string>

using namespace std;

typedef enum { ROULETTE_WHEEL_SELECTION=0, TOURNMENT_SELECTION } SelectionType;
typedef enum { UNIFORM_CROSSOVER=0, SINGLE_POINT_CROSSOVER, TWO_POINT_CROSSOVER } CrossoverType;
typedef enum { UNIFORM_MUTATION=0, GAUSSIAN_MUTATION} MutationType;

struct SelectionOpt {
  SelectionType type;
  int tournmentP;
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
    crossover.index1 = 0;
    crossover.index2 = 0;

    mutation.type = UNIFORM_MUTATION;
    mutation.uniformPerc = 0.25;

    mutation.mutationRate = 0.1;

    mutation.gaussianScale = 1.0;
    mutation.gaussianShrink = 1.0;

    verbose = false;
  }
};

class RealGenOptions {
public:
  RealGenOptions();
  ~RealGenOptions();

  void setOption(string option, float value);

private:
  GAOptions opt;
};


#endif