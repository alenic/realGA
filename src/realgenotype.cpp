#include "realgenotype.h"
#include <sstream>
#include <math.h>


RealGenotype::RealGenotype() {

}

RealGenotype::RealGenotype(int n) {
	gene.resize(n);
}

RealGenotype::RealGenotype(const RealGenotype &c) {
	gene = c.gene;
	fitness = c.fitness;
	LB = c.LB;
	UB = c.UB;
}

RealGenotype::~RealGenotype() {

}


void RealGenotype::setBounds(const vector<float> &lb, const vector<float> &ub) {
  LB = lb;
  UB = ub;
}

string RealGenotype::toString() {
	std::ostringstream os;
	os << "[";
	for(int i=0; i<gene.size()-1; i++) {
		os << gene[i] << ",";
	}
	os << gene[gene.size()-1] << "]";
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
    gene[i] = Stat::uniformRand(LB[i], UB[i]);
	}
}

void RealGenotype::uniformRandom(int i)
{
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::uniformRandom(int i) " << endl;
		exit(-1);
	}
	gene[i] = Stat::uniformRand(LB[i], UB[i]);
}

void RealGenotype::uniformLocalRandom(int i, float perc)
{
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::uniformLocalRandom(int i, float perc) " << endl;
		exit(-1);
	}
	float fraction = perc*(Stat::uniformRand()-0.5)*(UB[i]-LB[i]);
  // Mutate
	gene[i] += fraction;

	if(gene[i] < LB[i])
		gene[i] = LB[i];
	if(gene[i] > UB[i])
		gene[i] = UB[i];
}

void RealGenotype::gaussianLocalRandom(int i, float sigma) {
	if(i >= gene.size()){
		cerr << "ERROR: RealGenotype::gaussianLocalRandom(int i, float sigma) " << endl;
		exit(-1);
	}
	
	float r = Stat::gaussianRand(0.0, sigma)*(UB[i]-LB[i]);
  if (isnan(r) || isinf(r)) {
    cerr << "gaussianLocalRandom error!  r=" << r << endl;
    r = 0.0;
  }

  // Mutate
	gene[i] += r;
  if (gene[i] < LB[i])
    gene[i] = LB[i];
  if (gene[i] > UB[i])
    gene[i] = UB[i];
}

RealGenotype & RealGenotype::operator= ( const RealGenotype &c ) {
	gene = c.gene;
	fitness = c.fitness;
  LB = c.LB;
  UB = c.UB;
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