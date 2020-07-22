#include "realgenoptions.h"

GAOptions::GAOptions() {

}

RealGenOptions::RealGenOptions() {
  opt.populationSize = 100;

  opt.maxGenerations = 100 * opt.populationSize;

  opt.selection.type = ROULETTE_WHEEL_SELECTION;
  opt.selection.tournamentP = 4;
  opt.selection.sorting = true;
  opt.selection.elitismFactor = 0.1;

  opt.crossover.type = UNIFORM_CROSSOVER;
  opt.crossover.index1 = 0;
  opt.crossover.index2 = 0;

  opt.mutation.type = UNIFORM_MUTATION;
  opt.mutation.uniformPerc = 0.25;

  opt.mutation.mutationRate = 0.1;

  opt.mutation.gaussianScale = 1.0;
  opt.mutation.gaussianShrink = 1.0;

  opt.verbose = false;
}

RealGenOptions::~RealGenOptions() {

}

void RealGenOptions::setVerbose(bool value) {
  opt.verbose = value;
}

void RealGenOptions::setPopulationSize(size_t value) {
  opt.populationSize = value;
}

void RealGenOptions::setSolutionSize(size_t value) {
  opt.genesNumber = value;
}

void RealGenOptions::setLowerBounds(const vector<float> &lb) {
  opt.lowerBounds = lb;
}

void RealGenOptions::setUpperBounds(const vector<float> &ub) {
  opt.upperBounds = ub;
}

void RealGenOptions::setBounds(const vector<float> &lb, const vector<float> &ub) {
  opt.lowerBounds = lb;
  opt.upperBounds = ub;
}

void RealGenOptions::setFitnessFunction(FitnessFunction *f) {
  opt.fitnessFcnPtr = f;
}

void RealGenOptions::setMaxGenerations(size_t value) {
  opt.maxGenerations = value;
}

void RealGenOptions::setSelectionType(string value) {
  if (value == "roulette_wheel")
    opt.selection.type = ROULETTE_WHEEL_SELECTION;
  else if (value == "tournament")
    opt.selection.type = TOURNAMENT_SELECTION;
  else
    cerr << "setSelectionType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setSelectionTournamentP(size_t value) {
  opt.selection.tournamentP = value;
}

void RealGenOptions::setSorting(bool value) {
  opt.selection.sorting = value;
}

void RealGenOptions::setElitismFactor(float value) {
  opt.selection.elitismFactor = value;
}

void RealGenOptions::setMutationType(string value) {
  if (value == "uniform")
    opt.mutation.type = UNIFORM_MUTATION;
  else if (value == "gaussian")
    opt.mutation.type = GAUSSIAN_MUTATION;
  else
    cerr << "setMutationType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setUniformMutationPercentage(float value) {
  opt.mutation.uniformPerc = value;
}

void RealGenOptions::setMutationRate(float value) {
  opt.mutation.mutationRate = value;
}

void RealGenOptions::setMutationGaussianScaleShrink(float scale, float shrink) {
  opt.mutation.gaussianScale = scale;
  opt.mutation.gaussianShrink = shrink;
}

void RealGenOptions::setCrossoverType(string value) {
  if (value == "uniform")
    opt.crossover.type = UNIFORM_CROSSOVER;
  else if (value == "single_point")
    opt.crossover.type = SINGLE_POINT_CROSSOVER;
  else if (value == "two_point")
    opt.crossover.type = TWO_POINT_CROSSOVER;
  else
    cerr << "setCrossoverType(" << value << ") is an invalid option value" << endl;
}

void RealGenOptions::setSinglePointCrossoverIndex(size_t value) {
  opt.crossover.index1 = value;
}

void RealGenOptions::setTwoPointCrossoverIndexes(size_t i1, size_t i2) {
  opt.crossover.index1 = i1;
  opt.crossover.index2 = i2;
}


GAOptions RealGenOptions::getOptions(void) {
  return opt;
}