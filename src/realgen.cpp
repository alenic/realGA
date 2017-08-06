#include "realgen.h"

// Constructors
RealGen::RealGen(int np, int nx, float *lb, float *ub) {
	Np = np;
	Nx = nx;
	LB = lb;
	UB = ub;
	if(Np != population.size()) {
		population.resize(Np,Nx);
		newPopulation.resize(Np,Nx);
	}
	// Initialize standars deviations or gaussian mutation
	sigma = new float[nx];
	for(int i=0; i<Nx; i++) {
		sigma[i] = options.mutation.gaussianScale;
	}
	// Tournment indices array
	tourIndex = NULL;
	maxGenerations = 100*Nx;

	// statistics
	minFitness = -1;
	iminFitness = -1;
	maxFitness = -1;
	imaxFitness = -1;
	meanFitness = -1;
}

RealGen::RealGen(int np, int nx, float *lb, float *ub, GAOptions opt) : RealGen(np, nx, lb, ub) {
	setOptions(opt);

	if(options.verbose) {
		checkOptions();
	}
}

// Destructor
RealGen::~RealGen() {
	if(tourIndex) {
		delete []tourIndex;
	}
	delete [] sigma;
}


void RealGen::setOptions(GAOptions opt) {
	options = opt;
	if (tourIndex) {
		delete []tourIndex;
		tourIndex = new int[options.selection.tournmentP];
	} else {
		tourIndex = new int[options.selection.tournmentP];
	}
}

void RealGen::checkOptions() {
	cout << "------ checkOptions ------" << endl << endl;
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
			cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianScale = " << options.mutation.gaussianScale << endl;
			cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianShrink = " << options.mutation.gaussianShrink << endl;
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

// ========================================= Setter ======================================
void RealGen::setFitnessFunction(double (*f)(RealGenotype &, void *), void *par) {
	fitnessFcn = f;
	fitnessPar = par;
}

void RealGen::setSorting(bool value) {
	options.selection.sorting = value;
}


void RealGen::setPopulationSize(int n) {
	Np = n;
	population.clear();
	newPopulation.clear();
	if(Np != population.size()) {
		population.resize(Np,Nx);
	}
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

void RealGen::setMaxGenerations(int maxG) {
	maxGenerations = maxG;
}


// ========================================= Getter ======================================

int RealGen::getGeneration() {
	return generation;
}


double RealGen::evalFitness(RealGenotype &x) {
	RealGenotype g(x);
	g.bound(LB, UB);
	return fitnessFcn(g, fitnessPar);
}

double RealGen::getMeanFitness() {
	double meanF = 0.0;
	for(int i=0; i<Np; i++) {
		meanF += population[i].fitness;
	}
	return meanF / (double)Np;
}

RealGenotype RealGen::getBestChromosome() {
	RealGenotype best;
	best = population[iminFitness];
	best.bound(LB, UB);
	return best;
}

double RealGen::getMinFitness() {
	return minFitness;
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

void RealGen::evalMinFitness(){
	minFitness = population[0].fitness;
	iminFitness = 0;
	for(int i=0; i<Np; i++) {
		if(population[i].fitness < minFitness) {
			minFitness = population[i].fitness;
			iminFitness = i;
		}
	}
}

void RealGen::evalMaxFitness(){
	maxFitness = population[0].fitness;
	imaxFitness = 0;
	for(int i=0; i<Np; i++) {
		if(population[i].fitness > imaxFitness) {
			imaxFitness = population[i].fitness;
			imaxFitness = i;
		}
	}
}

string RealGen::populationToString() {
	std::ostringstream os;
	RealGenotype x_scaled;
	os << "============== Generation: " << generation << " ===================" << endl;
	for(int i=0; i<Np; i++) {
		x_scaled = population[i];
		x_scaled.bound(LB, UB);
		os << "[" << (i+1) << "] : "<< x_scaled.toString() << " -> Fitness " << x_scaled.fitness << endl;
	}
	return os.str();
}

void RealGen::evolve() {
	RealGenotype offspring(Nx);

	int index1, index2;
	size_t k=0;

	if(options.selection.type == ROULETTE_WHEEL_SELECTION) {
		sumFitnessRoulette();
	}

	if(options.selection.sorting) {
		while(k< options.selection.elitismFactor*Np) {
			newPopulation[k] = population[k];
			++k;
		}
	} else {
		// Keep only the best one
		newPopulation[k] = population[iminFitness];
		++k;
	}

	while(k < Np) {
		// Selection
		switch(options.selection.type) {
			case ROULETTE_WHEEL_SELECTION:
				rouletteWheelSelection(index1, index2);
			break;
			case TOURNMENT_SELECTION:
				tournmentSelection(options.selection.tournmentP, index1, index2);
			break;
		}
		
		// Crossover
		crossoverUniform(index1, index2, offspring);
		// Mutation
		switch(options.mutation.type) {
			case UNIFORM_MUTATION:
				uniformMutate(offspring, options.mutation.uniformPerc);
			break;
			case GAUSSIAN_MUTATION:
				gaussianLocalMutate(offspring);
			break;
		}
		
		offspring.fitness  = evalFitness(offspring);
		newPopulation[k] = offspring;
		++k;
	}

	if(options.selection.sorting) {
		partial_sort(newPopulation.begin(), newPopulation.begin()+int(options.selection.elitismFactor*Np), newPopulation.end());
	}

	if(options.mutation.type == GAUSSIAN_MUTATION) {
		for(int i=0; i<Nx; i++) {
			sigma[i] = options.mutation.gaussianScale*(1.0 - options.mutation.gaussianShrink*((float)generation/(float)maxGenerations))*0.1;
		}
	}

	evalMinFitness();
	evalMaxFitness();
	meanFitness = getMeanFitness();
	newPopulation.swap(population);
	generation++;
}

// ===================================== Init function =============================
void RealGen::initRandom() {
	generation=0;
	for(int i=0; i<Np; i++) {
		population[i].uniformRandom();
		population[i].fitness = evalFitness(population[i]);
	}
	if(options.selection.sorting) {
		sort(population.begin(), population.end());
	}

	// evaluate statistics
	evalMinFitness();
	evalMaxFitness();
	meanFitness = getMeanFitness();
}

void RealGen::evalPopulationFitness() {
	generation=0;
	for(int i=0; i<Np; i++) {
		population[i].fitness = evalFitness(population[i]);
	}
	if(options.selection.sorting) {
		partial_sort(population.begin(), population.begin()+int(options.selection.elitismFactor*Np), population.end());
	}
}

// ================= Selection =================================
void RealGen::rouletteWheelSelection(int &index1, int &index2) {
	rouletteWheel(index1, sumFitnessR*stat.uniformRand());
	rouletteWheel(index2, sumFitnessR*stat.uniformRand());
	if(index1 == index2) {
		if(index1 != Np-1)
			index2 = index1 + 1;
		else
			index2 = index1 - 1;
	}
}

double RealGen::sumFitnessRoulette() {
	double s=0.0;
	for(int i=0; i<Np; i++) {
		s += population[i].fitness;
	}
	sumFitnessR = Np*maxFitness-s;
}

void RealGen::rouletteWheel(int &index, float stop) {
	float sumP = 0.0;
	index = 0;
	for(int i=0; i<Np; i++) {
		sumP += (maxFitness-population[i].fitness);
		if(sumP > stop) {
			index = i;
			break; 
		}
	}
}

void RealGen::tournmentSelection(int p, int &index1, int &index2) {
	tournmentSelect(p, index1);
	tournmentSelect(p, index2);
	if(index1 == index2) {
		if(index1 != Np-1)
			index2 = index1 + 1;
		else
			index2 = index1 - 1;
	}
}

void RealGen::tournmentSelect(int p, int &iMin) {
	iMin = 0;
	double fMin = population[0].fitness;
	for(int i=0; i<p; i++) {
		tourIndex[i] = stat.uniformIndex(Np);
	}
	for(int i=0; i<p; i++) {
		double f=population[tourIndex[i]].fitness;
		if(f < fMin) {
			fMin = f;
			iMin = tourIndex[i];
		}
	}
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
			g.uniformLocalRandom(i, perc);
		}
	}
}

void RealGen::gaussianLocalMutate(RealGenotype &g) {
	for (size_t i=0; i<Nx; i++) {
		if(stat.uniformRand() < options.mutation.mutationRate) {
			g.gaussianLocalRandom(i, sigma[i]);
		}
	}
}
