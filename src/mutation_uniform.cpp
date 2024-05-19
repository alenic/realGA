#include "mutation.h"

UniformMutation::UniformMutation() : Mutation() {
}

UniformMutation::UniformMutation(float mutationRate, float mutationPerc) : Mutation(mutationRate, mutationPerc) {

}

UniformMutation::~UniformMutation() {}

void UniformMutation::mutate(RealChromosome &x, vector<float> &lb, vector<float> &ub) {
    for (int i=0; i<x.gene.size(); i++) {
        if(Stat::randUniform() < mMutationRate) {
            float fraction = mMutationPerc*(Stat::randUniform()-0.5)*(ub[i]-lb[i]);
            // Mutate
            x.gene[i] += fraction;
            if(x.gene[i] < lb[i]) x.gene[i] = lb[i];
            if(x.gene[i] > ub[i]) x.gene[i] = ub[i];
        }
    }
}