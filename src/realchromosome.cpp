/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "realchromosome.h"
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

RealChromosome & RealChromosome::operator= ( const RealChromosome &c ) {
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
    if(i >= gene.size()) {
        cerr << "ERROR: RealChromosome::randUniform(int i) " << endl;
        exit(-1);
    }
    gene[i] = Stat::randUniform(LB[i], UB[i]);
}

void RealChromosome::randUniformPerc(int i, float perc)
{
    if(i >= gene.size()) {
        cerr << "ERROR: RealChromosome::randUniformPerc(int i, float perc) " << endl;
        exit(-1);
    }
    float fraction = perc*(Stat::randUniform()-0.5)*(UB[i]-LB[i]);
    // Mutate
    gene[i] += fraction;

    if(gene[i] < LB[i])
        gene[i] = LB[i];
    if(gene[i] > UB[i])
        gene[i] = UB[i];
}

void RealChromosome::randGaussianPerc(int i, float perc) {
    if(i >= gene.size()){
        cerr << "ERROR: RealChromosome::randGaussianPerc(int i, float perc) " << endl;
        exit(-1);
    }
    float delta = UB[i] - LB[i];

    // 2 * sigma = delta / 2
    float sigma = perc * (delta / 2.0);

    if (sigma < 1e-6) {
        sigma = 1e-6;
    }

    float r = Stat::randGaussian(0.0, sigma);
    if (isnan(r) || isinf(r)) {
        cerr << "randGaussianPerc error!  r=" << r << endl;
        r = 0.0;
    }

    // Mutate
    gene[i] += r;
    if (gene[i] < LB[i])
        gene[i] = LB[i];
    if (gene[i] > UB[i])
        gene[i] = UB[i];
}


