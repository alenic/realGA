#include "mutation.h"

GaussianMutation::GaussianMutation() : Mutation() {
}

GaussianMutation::GaussianMutation(float mutationRate, float mutationPerc) : Mutation(mutationRate, mutationPerc) {

}

GaussianMutation::~GaussianMutation() {}

void GaussianMutation::mutate(RealChromosome &x, vector<float> &lb, vector<float> &ub) {
    for (int i=0; i<x.gene.size(); i++) {
        if(Stat::randUniform() < mMutationRate) {
            float delta = ub[i] - lb[i];
            // 2 * sigma = delta / 2
            float sigma = mMutationPerc * (delta / 4.0f);
            float r = Stat::randGaussian(0.0, sigma);
            if (isnan(r) || isinf(r)) {
                r = 0.0;
            }
            // Mutate
            x.gene[i] += r;
            if(x.gene[i] < lb[i]) x.gene[i] = lb[i];
            if(x.gene[i] > ub[i]) x.gene[i] = ub[i];
        }
    }
}

