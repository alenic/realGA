#include "crossover.h"

UniformCrossover::UniformCrossover(size_t chromosomeSize) : Crossover(chromosomeSize) {}

UniformCrossover::~UniformCrossover()
{
}

void UniformCrossover::crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring)
{
    for (size_t j = 0; j < mChromosomeSize; j++)
    {
        if (Stat::randUniform() < 0.5)
        {
            offspring.gene[j] = a.gene[j];
        }
        else
        {
            offspring.gene[j] = b.gene[j];
        }
    }
}