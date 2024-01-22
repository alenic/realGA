#include "chromosome.h"
#include <sstream>
#include <math.h>

RealChromosome::RealChromosome()
{
    fitness = 0.0;
}

RealChromosome::RealChromosome(int n)
{
    gene.resize(n);
}

RealChromosome::RealChromosome(const RealChromosome &c)
{
    gene = c.gene;
    fitness = c.fitness;
    LB = c.LB;
    UB = c.UB;
}

RealChromosome::~RealChromosome()
{

}

RealChromosome & RealChromosome::operator= (const RealChromosome &c) {
    gene = c.gene;
    fitness = c.fitness;
    LB = c.LB;
    UB = c.UB;
    return *this;
}

bool RealChromosome::operator<(const RealChromosome &c) const {
    return fitness < c.fitness;
}

bool RealChromosome::operator==(const RealChromosome &other) const {
    if(gene.size() != other.gene.size()) return false;
    for(int i=0; i<other.gene.size(); i++) {
        if(gene[i] != other.gene[i]) return false;
    }
    return true;
}

void RealChromosome::setBounds(const vector<float> &lb, const vector<float> &ub)
{
    LB = lb;
    UB = ub;
}

string RealChromosome::toString() {
    std::ostringstream os;
    os.precision(10);
    os << "[";
    for(int i=0; i<gene.size()-1; i++) {
        os << gene[i] << ",";
    }
    os << gene[gene.size()-1] << "]";
    return os.str();
}

float RealChromosome::distanceTo(RealChromosome &g) {
    float sse = 0.0;
    for(int i=0; i<gene.size(); i++) {
        float dx = gene[i]-g.gene[i];
        sse += dx*dx;
    }
    return sqrt(sse);
}

void RealChromosome::randUniform()
{
    for(int i=0; i<gene.size(); i++) {
        gene[i] = Stat::randUniform(LB[i], UB[i]);
    }
}

void RealChromosome::randUniform(int i)
{
    gene[i] = Stat::randUniform(LB[i], UB[i]);
}

void RealChromosome::uniformMutate(int i, float perc)
{
    float fraction = perc*(Stat::randUniform()-0.5)*(UB[i]-LB[i]);
    // Mutate
    gene[i] += fraction;

    if(gene[i] < LB[i])
        gene[i] = LB[i];
    if(gene[i] > UB[i])
        gene[i] = UB[i];
}

void RealChromosome::randGaussian(float mean, float sigma)
{
    for(int i=0; i<gene.size(); i++) {
        gene[i] = Stat::randGaussian(mean, sigma);
        if (gene[i] < LB[i]) gene[i] = LB[i];
        if (gene[i] > UB[i]) gene[i] = UB[i];
    }
}

void RealChromosome::randGaussian(int i, float mean, float sigma)
{
    gene[i] = Stat::randUniform(LB[i], UB[i]);
}

void RealChromosome::gaussianMutate(int i, float perc) {
    float delta = UB[i] - LB[i];

    // 2 * sigma = delta / 2
    float sigma = perc * (delta / 4.0f);

    float r = Stat::randGaussian(0.0, sigma);
    if (isnan(r) || isinf(r)) {
        r = 0.0;
    }

    // Mutate
    gene[i] += r;
    if (gene[i] < LB[i])
        gene[i] = LB[i];
    if (gene[i] > UB[i])
        gene[i] = UB[i];
}


