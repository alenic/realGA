#include "realgenotype.h"
#include <math.h>

RealGenotype::RealGenotype() {

}

RealGenotype::RealGenotype(int n) {
	gene.resize(n);
}

RealGenotype::RealGenotype(const RealGenotype &c) {
	gene = c.gene;
	fitness = c.fitness;
}

RealGenotype::~RealGenotype() {

}

string RealGenotype::toString() {
	std::ostringstream os;
	for(int i=0; i<gene.size(); i++) {
		os << gene[i] << " ";
	}
	return os.str();
}

double RealGenotype::distanceTo(RealGenotype &g) {
	double sse = 0.0;
	for(size_t i=0; i<gene.size(); i++) {
		double dx = gene[i]-g.gene[i];
		sse += dx*dx;
	}
	return sqrt(sse);
}


void RealGenotype::uniformRandom()
{
	for (size_t i=0; i<gene.size(); i++) {
		gene[i] = stat.uniformRand(LB[i], UB[i]);
	}
}

void RealGenotype::uniformRandom(int i)
{
	gene[i] = stat.uniformRand(LB[i], UB[i]);
}

void RealGenotype::uniformLocalRandom(int i, float perc)
{
	float width = (UB[i] - LB[i])/ 2.0;
	float var = width*perc*stat.uniformRand();

	if(stat.uniformRand() < 0.5) {
		gene[i] = gene[i] - var;
		if(gene[i] < LB[i])
			gene[i] = LB[i];
	} else {
		gene[i] = gene[i] + var;
		if(gene[i] > UB[i])
			gene[i] = UB[i];
	}
}

void RealGenotype::gaussianLocalRandom(int i, float sigma) {
	float r = stat.gaussianRand(0.0, sigma);
	//cout << r << " ";
	gene[i] += r;
	if(gene[i] < LB[i])
		gene[i] = LB[i];
	if(gene[i] > UB[i])
		gene[i] = UB[i];
}

RealGenotype & RealGenotype::operator= ( const RealGenotype &c ) {
	gene = c.gene;
	fitness = c.fitness;
	return *this;
}

bool RealGenotype::operator<(const RealGenotype &c) const {
	return fitness < c.fitness;
}

bool RealGenotype::operator==(const RealGenotype &other) const {
	if(gene.size() != other.gene.size()) return false;
	for(int i=0; i<other.gene.size(); i++) {
		if(gene[i] != other.gene[i]) return false;
	}
	return true;
}