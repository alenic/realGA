#include "realgen.h"

RealGen::RealGen(int np, int nx, vector<float> *lb, vector<float> *ub) {
	Np = np;
	Nx = nx;
	LB = lb;
	UB = ub;
	if(Np != population.size()) {
		population.resize(Np,Nx);
		newPopulation.resize(Np,Nx);
	}
	for(int i=0; i<Np; i++) {
		population[i].LB = lb;
		population[i].UB = ub;
	}
}

RealGen::RealGen(int np, int nx, vector<float> *lb, vector<float> *ub, GAOptions opt) {
	Np = np;
	Nx = nx;
	LB = lb;
	UB = ub;
	if(Np != population.size()) {
		population.resize(Np,Nx);
		newPopulation.resize(Np,Nx);
	}
	for(int i=0; i<Np; i++) {
		population[i].LB = lb;
		population[i].UB = ub;
	}

	options = opt;

	if(options.verbose) {
		checkOptions();
	}
}

void RealGen::checkOptions() {
	cout << "\n-- checkOptions --" << endl;
	switch (options.selection.type) {
		case ROULETTE_WHEEL_SELECTION:
			cout << "-> Selection: ROULETTE_WHEEL_SELECTION" << endl;
		break;
		case TOURNMENT_SELECTION:
			if(options.selection.tournmentP >= 2 && options.selection.tournmentP <= Np) {
				cout << "-> Selection: TOURNMENT_SELECTION : tournmentP = " << options.selection.tournmentP << endl;
			} else {
				cerr << "ERROR: options.selection.tournmentP must be an unsigned number between 2 and Np" << endl;
				exit(-1);
			}
		break;
	}

	if (options.selection.elitismFactor >= 0.0 && options.selection.elitismFactor <= 1.0) {
		cout << "-> Elitism Factor: " << options.selection.elitismFactor << endl;
	} else {
		cerr << "ERROR: options.selection.elitismFactor must be a number between 0.0 and 1.0" << endl;
		exit(-1);
	}

	if (options.selection.sorting) {
		cout << "-> Population Sorting : true" << endl;
	} else {
		cout << "-> Population Sorting : false" << endl;
	}

	cout << endl;

	switch (options.crossover.type) {
		case UNIFORM_CROSSOVER:
			cout << "-> Crossover: UNIFORM_CROSSOVER" << endl;
		break;
		case FIXED_CROSSOVER:
			if(options.crossover.fixedIndex >= 0 && options.crossover.fixedIndex < Np) {
				cout << "-> Crossover: FIXED_CROSSOVER : fixedIndex = " << options.crossover.fixedIndex << endl;
			} else {
				cerr << "ERROR: options.crossover.fixedIndex must be a number between 0 and Nx" << endl;
				exit(-1);
			}
			
		break;
		case SINGLE_POINT_CROSSOVER:
			cout << "-> Crossover: SINGLE_POINT_CROSSOVER" << endl;
		break;
		case TWO_POINT_CROSSOVER:
			cout << "-> Crossover: TWO_POINT_CROSSOVER" << endl;
		break;
	}

	cout << endl;

	switch (options.mutation.type) {
		case UNIFORM_MUTATION:
			if (options.mutation.uniformPerc >= 0.0 && options.mutation.uniformPerc <= 1.0) {
				cout << "-> Mutation: UNIFORM_MUTATION : uniformPerc = " << options.mutation.uniformPerc << endl;
			} else {
				cerr << "ERROR: options.selection.uniformPerc must be a number between 0.0 and 1.0" << endl;
				exit(-1);
			}
			
		break;
		case GAUSSIAN_MUTATION:
			if (options.mutation.gaussianPerc >= 0.0 && options.mutation.gaussianPerc <= 1.0) {
				cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianPerc = " << options.mutation.gaussianPerc << endl;
			} else {
				cerr << "ERROR: options.selection.gaussianPerc must be a number between 0.0 and 1.0" << endl;
				exit(-1);
			}
		break;
		case GAUSSIAN_SHRINK_MUTATION:
			cout << "-> Mutation: GAUSSIAN_SHRINK_MUTATION : gaussianShrink = " << options.mutation.gaussianShrink << endl;
		break;
	}

	if (options.mutation.mutationRate >= 0.0 && options.mutation.mutationRate <= 1.0) {
		cout << "-> MutationRate = " << options.mutation.mutationRate << endl;
	} else {
		cerr << "ERROR: options.mutation.mutationRate must be a number between 0.0 and 1.0" << endl;
		exit(-1);
	}
	cout << endl;
}

void RealGen::setOptions(GAOptions opt) {
	options = opt;
}

void RealGen::setFitnessFunction(double (*f)(RealGenotype &, void *), void *par) {
	fitnessFcn = f;
	fitnessPar = par;
}

void RealGen::setSorting(bool value) {
	options.selection.sorting = value;
}

void RealGen::setMutationRate(float value) {
	if(value >= 0 && value <= 1) {
		options.mutation.mutationRate = value;
	} else {
		cerr << "ERROR: options.mutation.mutationRate must be a number between 0.0 and 1.0" << endl;
		exit(-1);
	}
}

void RealGen::setElitismFactor(float value) {
	if(value >= 0.0 && value <= 1.0) {
		options.selection.elitismFactor = value;
	} else {
		cerr << "ERROR: options.selection.elitismFactor must be a number between 0.0 and 1.0" << endl;
		exit(-1);
	}
}

int RealGen::getEvolution() {
	return evolution;
}


double RealGen::getMeanFitness() {
	double meanF = 0.0;
	for(size_t i=0; i<Np; i++) {
		meanF += population[i].fitness / (double)Np;
	}
	return meanF;
}

RealGenotype *RealGen::getBestChromosome() {
	if(options.selection.sorting)
		return &population[0];
	return &population[iminFitness()];
}

double RealGen::getBestScore() {
	if(options.selection.sorting)
		return population[0].fitness;
	return population[iminFitness()].fitness;
}

double RealGen::diversity() {  // TODO
	/*
	// Calculate centroid
	for(size_t j=0; j<Nx; j++) {
		double c = 0.0;
		for(int i=0; i<Np; i++) {
			c += population[i].gene[j];
		}
		c /= Np;
	}*/
}

int RealGen::iminFitness(){
	double minF = 100000000000;
	int iminf=0;
	for(int i=0; i<Np; i++) {
		if(population[i].fitness < minF) {
			minF = population[i].fitness;
			iminf = i;
		}
	}
	return iminf;
}

// ===================================== Init function =============================
void RealGen::initRandom() {
	evolution=0;
	for(int i=0; i<Np; i++) {
		population[i].uniformRandom();
		population[i].fitness = fitnessFcn(population[i], fitnessPar);
	}
	if(options.selection.sorting) {
		sort(population.begin(), population.end());
	}
}

void RealGen::evaluateFitness() {
	evolution=0;
	for(int i=0; i<Np; i++) {
		population[i].fitness = fitnessFcn(population[i], fitnessPar);
	}
}


void RealGen::evolve() {
	RealGenotype offspring(Nx);
	offspring.LB = LB;
	offspring.UB = UB;

	int index1, index2;
	size_t k=0;
	sumFitnessRoulette();

	if(options.selection.sorting) {
		while(k< options.selection.elitismFactor*Np) {
			newPopulation[k] = population[k];
			++k;
		}
	} else {
		newPopulation[k] = population[iminFitness()];
		++k;
	}

	while(k < Np) {
		// Selection
		//tournmentSelection(4, p1, p2);
		rouletteWheelSelection(index1, index2);
		// Crossover
		crossoverUniform(index1, index2, offspring);
		// Mutation
		uniformMutate(offspring, 0.1);

		offspring.fitness  = fitnessFcn(offspring, fitnessPar);

		newPopulation[k] = offspring;
		++k;
	}

	if(options.selection.sorting) {
		partial_sort(newPopulation.begin(), newPopulation.begin()+int(options.selection.elitismFactor*Np), newPopulation.end());
	}

	population = newPopulation;
	evolution++;
}

// ================= Selection =================================
void RealGen::rouletteWheelSelection(int &index1, int &index2) {
	rouletteWheel(index1, sumFitnessR*stat.uniformRand());
	rouletteWheel(index2, sumFitnessR*stat.uniformRand());
	if(index1 == index2)
	{
		index2 = (index1 + 1) % Np;
	}
}

double RealGen::sumFitnessRoulette() {
	double s=0.0;
	for(int i=0; i<Np; i++) {
		s += population[i].fitness;
	}
	sumFitnessR = Np*population[Np-1].fitness-s;
}

void RealGen::rouletteWheel(int &index, float stop) {
	float fMax = population[Np-1].fitness;
	float sumP = 0.0;
	index = 0;
	for(int i=0; i<Np; i++) {
		sumP += (fMax-population[i].fitness);
		if(sumP > stop) {
			index = i;
			break; 
		}
	}
}

void RealGen::tournmentSelection(int p, RealGenotype &p1, RealGenotype &p2) {
	tournmentSelect(p, p1);
	tournmentSelect(p, p2);
}

void RealGen::tournmentSelect(int p, RealGenotype &g) {
	int *rindex = new int[p];
	double fMin = 1.8e16, iMin = 0;
	for(int i=0; i<p; i++) {
		rindex[i] = stat.uniformRand()*Np;
	}
	for(int i=0; i<p; i++) {
		double f=population[rindex[i]].fitness;
		if(f < fMin) {
			fMin = f;
			iMin = rindex[i];
		}
	}
	g = population[iMin];
	delete []rindex;
}
//==================================== Crossover ==================
void RealGen::crossoverUniform(int index1, int index2, RealGenotype &c) {
	for(size_t i=0; i<Nx; i++) {
		if(stat.uniformRand()<0.5) {
			c.gene[i] = population[index1].gene[i];
		} else {
			c.gene[i] = population[index2].gene[i];
		}
	}
}

void RealGen::crossoverFixed(int index1, int index2, RealGenotype &c, int n) {
	for(size_t i=0; i<n; i++) {
		c.gene[i] = population[index1].gene[i];
	}
	for(size_t i=n; i<Nx; i++) {
		c.gene[i] = population[index2].gene[i];
	}
}
//===================================== Mutation ======================
void RealGen::uniformMutate(RealGenotype &g, float perc) {
	for (size_t i=0; i<Nx; i++) {
		if(stat.uniformRand() < options.mutation.mutationRate) {
			if(perc < 1.0)
				g.uniformLocalRandom(i, perc);
			else
				g.uniformRandom(i);
		}
	}
}

void RealGen::gaussianLocalMutate(RealGenotype &g, float stDev) {
	for (size_t i=0; i<Nx; i++) {
		if(stat.uniformRand() < options.mutation.mutationRate) {
			g.gaussianLocalRandom(i, stDev);
		}
	}
}


void RealGen::setPopulationSize(int n) {
	Np = n;
	population.clear();
	newPopulation.clear();
	if(Np != population.size()) {
		population.resize(Np,Nx);
	}
}

string RealGen::populationToString() {
	std::ostringstream os;
	os << "============== Evolution " << evolution << " ===================" << endl;
	for(int i=0; i<Np; i++) {
		os << "[" << (i+1) << "] : "<< population[i].toString() << " -> Fitness " << population[i].fitness << endl;
	}
	return os.str();
}