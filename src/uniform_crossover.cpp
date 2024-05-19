#include "crossover.h"


UniformCrossover::UniformCrossover() {

}

UniformCrossover::~UniformCrossover() {

}

void UniformCrossover::crossover(RealChromosome &a, RealChromosome &b, RealChromosome &offspring) {
    size_t n = a.gene.size();
    for(int j=0; j<n; j++) {
        if(Stat::randUniform()<0.5) {
            offspring.gene[j] = a.gene[j];
        } else {
            offspring.gene[j] = b.gene[j];
        }
    }
}