#include "options.h"

RealGAOptions::RealGAOptions()
{
    populationSize = 100;
    chromosomeSize = -1;
    verbose = NO_VERBOSE;
    seed = 42;
    elitismFactor = 0.1;
    mutateDuplicatedFitness = true;

    // SelectionOpt
    selectionType = ROULETTE_WHEEL_SELECTION;
    selectionTournamentSize = 16;
    selectionTournamentProbability = 0.85;

    // CrossoverOpt
    crossoverType = UNIFORM_CROSSOVER;
    crossoverindexA = -1;

    // MutationOpt
    mutationType = UNIFORM_MUTATION;
    mutationRate = 0.1;

    // Uniform mutation
    mutationUniformPercDelta = 0.01; // 100 iterations decay to 0.01
    mutationUniformPercMin = 0.001;  // 1000 iterations

    // Gaussian mutation
    mutationGaussianPercDelta = 0.01; // 100 iterations decay to 0.01
    mutationGaussianPercMin = 0.001;  // 1000 iterations
}

RealGAOptions::~RealGAOptions()
{
}

void RealGAOptions::setPopulationSize(size_t value)
{
    REALGA_ERROR(value <= 0, "Population size must be > 0");
    populationSize = value;
}

void RealGAOptions::setChromosomeSize(size_t value)
{
    REALGA_ERROR(value <= 0, "Chromosome size must be > 0");
    chromosomeSize = value;
}

void RealGAOptions::setLowerBounds(const vector<float> &lb)
{
    lowerBounds = lb;
}

void RealGAOptions::setUpperBounds(const vector<float> &ub)
{
    upperBounds = ub;
}

void RealGAOptions::setBounds(const vector<float> &lb, const vector<float> &ub)
{
    setLowerBounds(lb);
    setUpperBounds(ub);
}

void RealGAOptions::setVerbose(string value)
{
    if (value == "none")
        verbose = NO_VERBOSE;
    else if (value == "soft")
        verbose = SOFT_VERBOSE;
    else if (value == "hard")
        verbose = HARD_VERBOSE;
    else
        REALGA_ERROR(1, value << " is an invalid verbose type");
}

void RealGAOptions::setElitismFactor(float value)
{
    REALGA_ERROR((value < 0.0) || (value > 1.0), "elitism factor must be in [0,1]");
    elitismFactor = value;
}

void RealGAOptions::setMutateDuplicatedFitness(bool value)
{
    mutateDuplicatedFitness = value;
}

void RealGAOptions::setSeed(unsigned int value)
{
    seed = value;
}

void RealGAOptions::setSelectionType(string value)
{
    if (value == "roulette")
        selectionType = ROULETTE_WHEEL_SELECTION;
    else if (value == "tournament")
        selectionType = TOURNAMENT_SELECTION;
    else
        REALGA_ERROR(1, value << " is an invalid selection type");
}

void RealGAOptions::setSelectionTournamentSize(int value)
{
    REALGA_ERROR(value <= 0, "Tournament size must be > 0");
    selectionTournamentSize = value;
}

void RealGAOptions::setSelectionTournamentProbability(float value)
{
    REALGA_ERROR((value < 0.0) || (value > 1.0), "tournament probability must be in [0,1]");
    selectionTournamentProbability = value;
}

void RealGAOptions::setMutationType(string value)
{
    if (value == "uniform")
        mutationType = UNIFORM_MUTATION;
    else if (value == "gaussian")
        mutationType = GAUSSIAN_MUTATION;
    else
        REALGA_ERROR(1, value << " is an invalid mutation type");
}

void RealGAOptions::setMutationRate(float value)
{
    REALGA_ERROR((value < 0.0) || (value > 1.0), "mutation rate must be in [0,1]");
    mutationRate = value;
}

void RealGAOptions::setMutationUniformPerc(float percDelta, float percMin)
{
    REALGA_ERROR((percDelta < 0.0) || (percDelta > 1.0), "uniform mutation delta percentage must be in [0,1]");
    REALGA_ERROR((percMin < 0.0) || (percMin > 1.0), "uniform mutation min percentage must be in [0,1]");
    mutationUniformPercDelta = percDelta;
    mutationUniformPercMin = percMin;
}

void RealGAOptions::setMutationGaussianPerc(float percDelta, float percMin)
{
    REALGA_ERROR((percDelta < 0.0) || (percDelta > 1.0), "gaussian mutation delta percentage must be in [0,1]");
    REALGA_ERROR((percMin < 0.0) || (percMin > 1.0), "gaussian mutation min percentage must be in [0,1]");
    mutationGaussianPercDelta = percDelta;
    mutationGaussianPercMin = percMin;
}

void RealGAOptions::setCrossoverType(string value)
{
    if (value == "uniform")
        crossoverType = UNIFORM_CROSSOVER;
    else
        REALGA_ERROR(1, value << " is an invalid crossover type");
}

void RealGAOptions::setSinglePointCrossoverIndex(int value)
{
    crossoverindexA = value;
}

void RealGAOptions::checkOptions()
{
    // Check if chromosome size is compatible with options
    REALGA_ERROR((lowerBounds.size() != chromosomeSize),
                 "lower bounds size " << lowerBounds.size() << " must equals chromosome size " << chromosomeSize);
    REALGA_ERROR((upperBounds.size() != chromosomeSize),
                 "upper bounds size " << upperBounds.size() << " must equals chromosome size " << chromosomeSize);

    // check population size
    if (selectionType == TOURNAMENT_SELECTION)
    {
        REALGA_ERROR(populationSize <= selectionTournamentSize, "tournament size " << selectionTournamentSize << " must be less than population size " << populationSize);
    }
}