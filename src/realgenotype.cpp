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
	os << "(";
	for(int i=0; i<gene.size(); i++) {
		os << gene[i] << ",";
	}
	os << ")";
	return os.str();
}

void RealGenotype::bound(float *lb, float *ub) {
	for(int i=0; i<gene.size(); i++) {
		gene[i] = gene[i]*(ub[i] - lb[i]) + lb[i];
	}
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
		gene[i] = stat.uniformRand();
	}
}

void RealGenotype::uniformRandom(int i)
{
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::uniformRandom(int i) " << endl;
		exit(-1);
	}
	gene[i] = stat.uniformRand();
}

void RealGenotype::uniformLocalRandom(int i, float perc)
{
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::uniformLocalRandom(int i, float perc) " << endl;
		exit(-1);
	}
	float d = perc*stat.uniformRand();
	gene[i] = gene[i] + d - perc*0.5;
	if(gene[i] < 0.0)
		gene[i] = 0.0;
	if(gene[i] > 1.0)
		gene[i] = 1.0;
}

void RealGenotype::gaussianLocalRandom(int i, float sigma) {
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::gaussianLocalRandom(int i, float sigma) " << endl;
		exit(-1);
	}
	
	float r = stat.gaussianRand(0.0, sigma);
	gene[i] += r;
	if(gene[i] < 0.0)
		gene[i] = 0.0;
	if(gene[i] > 1.0)
		gene[i] = 1.0;
}

RealGenotype & RealGenotype::operator= ( const RealGenotype &c ) {
	gene = c.gene;
	fitness = c.fitness;
	stat = c.stat;
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