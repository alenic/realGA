#include "realgen.h"

// Constructors
RealGen::RealGen(RealGenOptions opt) {
  tourIndex = nullptr;
  // statistics
  minFitness = -1;
  iminFitness = -1;
  maxFitness = -1;
  imaxFitness = -1;
  meanFitness = -1;
  
  setOptions(opt.getOptions());
}

// Destructor
RealGen::~RealGen() {
  if(tourIndex != nullptr) {
    delete []tourIndex;
  }
  delete [] sigma;
}


void RealGen::setOptions(GAOptions opt) {
  options = opt;
  Np = options.populationSize;
  Nx = options.genesNumber;
  LB = options.lowerBounds;
  UB = options.upperBounds;
  if (Np != population.size()) {
	  population.resize(Np, Nx);
	  newPopulation.resize(Np, Nx);
  }
  // Initialize standars deviations or gaussian mutation
  sigma = new float[Nx];
  for (int i = 0; i<Nx; i++) {
	  sigma[i] = options.mutation.gaussianScale;
  }
  // Tournament indices array
  if (tourIndex != nullptr) {
    delete[]tourIndex;
  }
  tourIndex = new int[options.selection.tournamentP];
  maxGenerations = opt.maxGenerations;

  setFitnessFunction(options.fitnessFcn, options.fitnessPar);

  if (options.verbose) {
	  checkOptions();
  }
}

void RealGen::checkOptions() {
  cout << "------ checkOptions ------" << endl << endl;
  switch (options.selection.type) {
    case ROULETTE_WHEEL_SELECTION:
      cout << "-> Selection: ROULETTE_WHEEL_SELECTION" << endl;
    break;
    case TOURNAMENT_SELECTION:
      if(options.selection.tournamentP >= 2 && options.selection.tournamentP <= Np) {
        cout << "-> Selection: TOURNAMENT_SELECTION : tournamentP = " << options.selection.tournamentP << endl;
      } else {
        cerr << "ERROR: options.selection.tournamentP must be an unsigned number between 2 and Np" << endl;
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
    case SINGLE_POINT_CROSSOVER:
      if(options.crossover.index1 >= 0 && options.crossover.index1 < Nx) {
        cout << "-> Crossover: SINGLE_POINT_CROSSOVER : index1 = " << options.crossover.index1 << endl;
      } else {
        cerr << "ERROR: options.crossover.index1 must be a number between 0 and Nx" << endl;
        exit(-1);
      }
    break;
    case TWO_POINT_CROSSOVER:
      if(options.crossover.index1 >= 0 && options.crossover.index1 < Nx && 
         options.crossover.index2 >= 0 && options.crossover.index2 < Nx) {
        cout << "-> Crossover: TWO_POINT_CROSSOVER : index1 = " << options.crossover.index1 << "index2 = " << options.crossover.index2 << endl;
      } else {
        cerr << "ERROR: crossover_index1 and crossover_index2 must be a number between 0 and Nx" << endl;
        exit(-1);
      }
      if(options.crossover.index1 > options.crossover.index2) {
        cerr << "ERROR: crossover_index1 <= crossover_index2 must be satisfied" << endl;
        exit(-1);
      }
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


void RealGen::setSeed(unsigned int seed) {
  stat.setSeed(seed);
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
  double I = 0.0;
  // Compute the centroid
  for(size_t j=0; j<Nx; j++) {
    double ci = 0.0;
    for(int i=0; i<Np; i++) {
      ci += population[i].gene[j];
    }
    ci /= Np;
    for(int i=0; i<Np; i++) {
      double dx = population[i].gene[j] - ci;
      I += dx*dx;
    }
  }
  return I;
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
      case TOURNAMENT_SELECTION:
        tournamentSelection(options.selection.tournamentP, index1, index2);
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
        gaussianMutate(offspring);
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


void RealGen::initMutate(vector<float> &gene, float sigma) {
  generation=0;
  RealGenotype g(Nx);
  for(int i=0; i<Nx; i++) {
    if(gene[i] > UB[i]) gene[i] = UB[i];
    if(gene[i] < LB[i]) gene[i] = LB[i];
    g.gene[i] = (gene[i]-LB[i])/(UB[i]-LB[i]);
  }
  population[0] = g;
  population[0].fitness = evalFitness(population[0]);
  for(int i=1; i<Np; i++) {
    population[i] = g;
    for(int j=0; j<Nx; ++j) {
      population[i].gaussianLocalRandom(j, sigma);
    }
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

void RealGen::sumFitnessRoulette() {
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

void RealGen::tournamentSelection(int p, int &index1, int &index2) {
  tournamentSelect(p, index1);
  tournamentSelect(p, index2);
  if(index1 == index2) {
    if(index1 != Np-1)
      index2 = index1 + 1;
    else
      index2 = index1 - 1;
  }
}

void RealGen::tournamentSelect(int p, int &iMin) {
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

void RealGen::gaussianMutate(RealGenotype &g) {
  for (size_t i=0; i<Nx; i++) {
    if(stat.uniformRand() < options.mutation.mutationRate) {
      g.gaussianLocalRandom(i, sigma[i]);
    }
  }
}
