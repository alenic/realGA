#include "mutation.h"


Mutation::Mutation() {
    mMutationRate = 0.1;
    mMutationPerc = 1.0;
}

Mutation::Mutation(float mutationRate, float mutationPerc) {
    mMutationRate = mutationRate;
    mMutationPerc = mutationPerc;
}

Mutation::~Mutation() {}

void Mutation::setMutationRate(float mutationRate) {
    mMutationRate = mutationRate;
}

void Mutation::setMutationPercentage(float mutationPerc) {
    mMutationPerc = mutationPerc;
}

float Mutation::getMutationRate() {
    return mMutationRate;
}

float Mutation::getMutationPercentage() {
    return mMutationPerc;
}