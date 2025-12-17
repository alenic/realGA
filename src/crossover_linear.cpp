#include "crossover.h"

LinearCrossover::LinearCrossover(size_t chromosomeSize) : Crossover(chromosomeSize) {}

LinearCrossover::~LinearCrossover()
{
}

void LinearCrossover::crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring)
{   
    // Safety check: ensure offspring has the correct size
    REALGA_ERROR(offspring.gene.size() != mChromosomeSize, "offspring.size() != chromosomeSize");
    
    float alpha = Stat::randUniform();
    for (size_t j = 0; j < mChromosomeSize; j++)
    {
        offspring.gene[j] = alpha*a.gene[j] + (1.0f-alpha)*b.gene[j];
    }
}