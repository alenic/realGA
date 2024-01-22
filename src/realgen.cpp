/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "realgen.h"

RealGen::RealGen()
// Default Constructor
{
    // Reset pointers and fitness values
    mFitnessFcn = nullptr;
    mGeneration = 0;
    mSelectionAlgorithm = nullptr;
    mGaussianPerc = 1.0f;
}

RealGen::~RealGen()
// Destructor
{
    delete mSelectionAlgorithm;
}

void RealGen::checkOptions()
// Check and print options
{
    cout << "------ checkOptions ------" << endl << endl;
    switch (mOptions.selectionType) {
    case ROULETTE_WHEEL_SELECTION:
        cout << "-> Selection: ROULETTE_WHEEL_SELECTION" << endl;
        break;
    case TOURNAMENT_SELECTION:
        if(mOptions.selectionTournamentSize >= 2 && mOptions.selectionTournamentSize <= mOptions.populationSize) {
            cout << "-> Selection: TOURNAMENT_SELECTION : tournamentP = " << mOptions.selectionTournamentSize << endl;
        } else {
            cerr << "ERROR: options.selection.tournamentP must be an unsigned number between 2 and Np" << endl;
            exit(-1);
        }
        break;
    }

    if (mOptions.selectionElitismFactor >= 0.0 && mOptions.selectionElitismFactor <= 1.0) {
        cout << "-> Elitism Factor: " << mOptions.selectionElitismFactor << endl;
    } else {
        cerr << "ERROR: options.selection.elitismFactor must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }

    cout << endl;

    switch (mOptions.crossoverType) {
    case UNIFORM_CROSSOVER:
        cout << "-> Crossover: UNIFORM_CROSSOVER" << endl;
        break;
    case SINGLE_POINT_CROSSOVER:
        if(mOptions.crossoverindexA >= 0 && mOptions.crossoverindexA < mOptions.chromosomeSize) {
            cout << "-> Crossover: SINGLE_POINT_CROSSOVER : indexA = " << mOptions.crossoverindexA << endl;
        } else {
            cerr << "ERROR: options.crossover.indexA must be a number between 0 and Nx" << endl;
            exit(-1);
        }
        break;
    case TWO_POINT_CROSSOVER:
        if(mOptions.crossoverindexA >= 0 && mOptions.crossoverindexA < mOptions.chromosomeSize &&
                mOptions.crossoverindexB >= 0 && mOptions.crossoverindexB < mOptions.chromosomeSize) {
            cout << "-> Crossover: TWO_POINT_CROSSOVER : indexA = " << mOptions.crossoverindexA << "indexB = " << mOptions.crossoverindexB << endl;
        } else {
            cerr << "ERROR: crossover_indexA and crossover_indexB must be a number between 0 and Nx" << endl;
            exit(-1);
        }
        if(mOptions.crossoverindexA > mOptions.crossoverindexB) {
            cerr << "ERROR: crossover_indexA <= crossover_indexB must be satisfied" << endl;
            exit(-1);
        }
        break;
    }

    cout << endl;

    switch (mOptions.mutationType) {
    case UNIFORM_MUTATION:
        if (mOptions.mutationUniformPerc >= 0.0 && mOptions.mutationUniformPerc <= 1.0) {
            cout << "-> Mutation: UNIFORM_MUTATION : uniformPerc = " << mOptions.mutationUniformPerc << endl;
        } else {
            cerr << "ERROR: options.selection.uniformPerc must be a number between 0.0 and 1.0" << endl;
            exit(-1);
        }

        break;
    case GAUSSIAN_MUTATION:
        cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianPercDelta = " << mOptions.mutationGaussianPercDelta << endl;
        cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianPercMin = " << mOptions.mutationGaussianPercMin << endl;
        break;
    }

    if (mOptions.mutationRate >= 0.0 && mOptions.mutationRate <= 1.0) {
        cout << "-> MutationRate = " << mOptions.mutationRate << endl;
    } else {
        cerr << "ERROR: options.mutation.mutationRate must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }
    cout << endl;
}

// ========================================= Setter ======================================

void RealGen::resetPopulation()
{
    // Check if populationSize has changed
    if(mOptions.populationSize != mPopulation.size()) {
        mPopulation.clear();
        mNewPopulation.clear();
        mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mFitnessValues.resize(mOptions.populationSize);
    }
    // Check if chromosomeSize has changed
    if(mPopulation.size() != 0) {
        if(mOptions.chromosomeSize != mPopulation[0].gene.size()) {
            mPopulation.clear();
            mNewPopulation.clear();
            mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mFitnessValues.resize(mOptions.populationSize);
        }
    }
}

void RealGen::resetGaussianMutationPerc()
// Initialize standars deviations for gaussian mutation
{
    mGaussianPerc = 1.0f;
}

void RealGen::restart() {
    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        resetGaussianMutationPerc();
    }
    mGeneration = 0;
    if (mOptions.verbose>0) {
        checkOptions();
    }
}

void RealGen::init(RealGenOptions &opt, FitnessFunction *func, bool keepState)
// Set custom options
{
    // set options
    mOptions = opt;
    // set fitness function
    mFitnessFcn = func;

    if(keepState) {
        if(mPopulation.size() == 0) {
            cerr << "ERROR: setOptions: reset is false, but chromosome population size is 0" << endl;
            exit(-1);
        } else if(mOptions.populationSize != mPopulation.size()) {
            cerr << "ERROR: setOptions: reset is false, but population size " << mPopulation.size()
            << " is different from option population size " << mOptions.populationSize << endl;
            exit(-1);
        }
        
        if(mOptions.chromosomeSize != mPopulation[0].gene.size()) {
            cerr << "ERROR: setOptions: reset is false, but chromosome size " << mPopulation[0].gene.size()
            << " is different from option chromosome size " << mOptions.chromosomeSize << endl;
            exit(-1);
        }

    } else {
        // reset population size
        resetPopulation();
        
        // Set bounds to all chromosomes
        for (int i = 0; i < mPopulation.size(); ++i) {
            mPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
            mNewPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
        }
        
        // Initialize gaussian standard deviation
        if(mOptions.mutationType == GAUSSIAN_MUTATION) {
            resetGaussianMutationPerc();
        }
        
        // Allocate the selection algorithm
        switch(mOptions.selectionType) {
            case ROULETTE_WHEEL_SELECTION:
                mSelectionAlgorithm = new RouletteWheelSelection(mOptions.populationSize);
                break;
            case TOURNAMENT_SELECTION:
                mSelectionAlgorithm = new TournamentSelection(mOptions.populationSize);
                ((TournamentSelection *)mSelectionAlgorithm)->setTournamentSize(mOptions.selectionTournamentSize);
                ((TournamentSelection *)mSelectionAlgorithm)->setSelectionProbability(mOptions.selectionTournamentProbability);
                break;
        }

        if (mOptions.verbose>0) {
            checkOptions();
        }
        Stat::setSeed(mOptions.seed);
        mGeneration = 0;
    }
    mElitismNumber = (int)(mOptions.selectionElitismFactor * mOptions.populationSize);
}

void RealGen::setFitnessFunction(FitnessFunction *f) 
{
    mFitnessFcn = f;
}

void RealGen::fillFitnessValues(vector<RealChromosome> &population) {
    for(int i=0; i<population.size(); i++) {
        mFitnessValues[i] = population[i].fitness;
    }
}
// ========================================= Getter ======================================

int RealGen::getGeneration() {
    return mGeneration;
}


float RealGen::evalFitness(const RealChromosome &x) {
    return mFitnessFcn->eval(x);
}

RealChromosome RealGen::getBestChromosome() {
    RealChromosome best;
    int minIndex=0;
    float minValue=mPopulation[0].fitness;
    for(int i=0; i<mPopulation.size(); i++) {
        float value = mPopulation[i].fitness;
        if(value < minValue) {
            minValue = value;
            minIndex = i;
        }
    }
    best = mPopulation[minIndex];
    return best;
}

/*
float RealGen::getDiversity() {  //  TODO
    float I = 0.0;
    // Compute the centroid
    for(int j=0; j<mOptions.chromosomeSize; j++) {
        float ci = 0.0;
        for(int i=0; i<mOptions.populationSize; i++) {
            ci += mPopulation[i].gene[j];
        }
        ci /= mOptions.populationSize;
        for(int i=0; i<mOptions.populationSize; i++) {
            float dx = mPopulation[i].gene[j] - ci;
            I += dx*dx;
        }
    }
    return I;
}
*/

vector<RealChromosome> RealGen::getPopulation() {
    return mPopulation;
}

string RealGen::populationToString() {
    std::ostringstream os;
    RealChromosome x;
    os << "============== Generation: " << mGeneration << " ===================" << endl;
    for(int i=0; i<mOptions.populationSize; i++) {
        x = mPopulation[i];
        os << "[" << (i+1) << "] : "<< x.toString() << " -> Fitness " << x.fitness << endl;
    }
    return os.str();
}


bool RealGen::checkChromosome(RealChromosome &chromosome) {
    for (int j = 0; j < mOptions.chromosomeSize; ++j) {
        if (isnan(chromosome.gene[j]) || isinf(chromosome.gene[j]) ){
            cout << "checkGene Failed:" << endl;
            cout << chromosome.toString() << endl;
            cout << " has gene[" << j << "] = " << chromosome.gene[j] << " not normal" << endl;
            return false;
        }
        if (chromosome.gene[j] < mOptions.lowerBounds[j]) {
            cout << "checkGene Failed:" << endl;
            cout << chromosome.toString() << endl;
            cout << " has gene[" << j << "] = " << chromosome.gene[j] << " < LB = " << mOptions.lowerBounds[j] << endl;
            return false;
        }
        if (chromosome.gene[j] > mOptions.upperBounds[j]) {
            cout << "checkGene Failed:" << endl;
            cout << chromosome.toString() << endl;
            cout << " has gene[" << j << "] = " << chromosome.gene[j] << " > UB = " << mOptions.upperBounds[j] << endl;
            return false;
        }
    }
    return true;
}


void RealGen::checkPopulation() {
    for (int i = 0; i < mOptions.populationSize; i++) {
        if (!checkChromosome(mPopulation[i])) {
            cout << "checkPopulation Failed: Chromosome " << i << endl;
            exit(-1);
        }
    }
}

#define SORT 0
// ==================================================== Evolve ====================================================
void RealGen::evolve() {
    // Allocate offspring (gene after crossover and mutation)
    RealChromosome offspring(mOptions.chromosomeSize);
    int selectedIndexA, selectedIndexB;
    int k=0;
    int countElite=0;

    // fill mFitnessValues to accelerate some functions
    fillFitnessValues(mPopulation);
    if(!SORT)
    {   

            mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize-1, mElitismNumber);
            //cout << "original"<<endl;
            //for(int i=0;i<mFitnessValues.size();i++)cout << mFitnessValues[i] << ",";
            //cout << endl;
            //cout << "MBEST " << mKthSmallestFitness << endl << endl;

    }

    mSelectionAlgorithm->init(mFitnessValues);
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);
    if(SORT)
    {

    // Keep the 0:elitismIndex elements in the new population
    while (k < mElitismNumber) {
        mNewPopulation[k] = mPopulation[k];
        ++k;
    }

    } // SORT
    else
    {
        k = 0;
        countElite = 0;
    }

    // Generate New Population
    while(k < mOptions.populationSize) {

        if(!SORT)
        {
        if((mFitnessValues[k] <= mKthSmallestFitness) && (countElite <= mElitismNumber)) {
            //cout << mFitnessValues[k] << " < " << mKthSmallestFitness << endl;
            mNewPopulation[k] = mPopulation[k];
            ++k;
            ++countElite;
            continue;
        }

        }


        // Selection
        mSelectionAlgorithm->select(mFitnessValues, selectedIndexA, selectedIndexB);

        // Crossover
        switch(mOptions.crossoverType) {
            case UNIFORM_CROSSOVER:
                crossoverUniform(selectedIndexA, selectedIndexB, offspring);
                break;
            case SINGLE_POINT_CROSSOVER:
                crossoverFixed(selectedIndexA, selectedIndexB, offspring, mOptions.crossoverindexA);
                break;
        }
        
        // Mutation
        switch(mOptions.mutationType) {
            case UNIFORM_MUTATION:
                uniformMutate(offspring, mOptions.mutationRate, mOptions.mutationUniformPerc);
                break;
            case GAUSSIAN_MUTATION:
                gaussianMutate(offspring, mOptions.mutationRate, mGaussianPerc);
                break;
        }

        offspring.fitness = evalFitness(offspring);
        mNewPopulation[k] = offspring;
        ++k;
    }

    
    
    if(SORT)
    {
    
    partial_sort(mNewPopulation.begin(), mNewPopulation.begin()+mElitismNumber, mNewPopulation.end());

    }
    else
    {
    /*
    if(mElitismNumber > 0) {
        //for(int i=0;i<mFitnessValues.size();i++) cout << mNewPopulation[i].fitness << endl;
        fillFitnessValues(mNewPopulation);
        
        //exit(-1);
        // it doesn't overwrite mFitnessValues
        mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize-1, mElitismNumber);
    }*/

    }

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        if (mGaussianPerc > mOptions.mutationGaussianPercMin) {
            mGaussianPerc = 1.0f - mOptions.mutationGaussianPercDelta*(float)mGeneration;
        } else {
            mGaussianPerc = mOptions.mutationGaussianPercMin;
        }
    }

    for(int i=0; i<mPopulation.size(); i++) {
        mPopulation[i] = mNewPopulation[i];
    }
    
    mGeneration++;
}

// ===================================== Init function =============================
void RealGen::popInitRandUniform()
// Init random uniform population
{
    for(int i=0; i<mOptions.populationSize; i++) {
        mPopulation[i].randUniform();
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    
    if(SORT)partial_sort(mPopulation.begin(), mPopulation.begin()+mElitismNumber, mPopulation.end());
}

void RealGen::popInitRandGaussian(float mean, float sigma)
// Init random gaussian population
{
    for(int i=0; i<mOptions.populationSize; i++) {
        mPopulation[i].randGaussian(mean, sigma);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }

    if(SORT)partial_sort(mPopulation.begin(), mPopulation.begin()+mElitismNumber, mPopulation.end());
}


void RealGen::popInitGaussianMutate(vector<float> &gene, float mutatioRate, float perc)
// Init population from a given chromosome and mutate it
{
    RealChromosome g(mOptions.chromosomeSize);
    g.setBounds(mOptions.lowerBounds, mOptions.upperBounds);
    for(int i=0; i<mOptions.chromosomeSize; i++) {
        g.gene[i] = gene[i];
    }
    mPopulation[0] = g;
    mPopulation[0].fitness = evalFitness(mPopulation[0]);
    for(int i=1; i<mOptions.populationSize; i++) {
        mPopulation[i] = g;
        gaussianMutate(mPopulation[i], mutatioRate, perc);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }

    if(SORT)partial_sort(mPopulation.begin(), mPopulation.begin()+mElitismNumber, mPopulation.end());
}

// TODO
void RealGen::popInitSetChromosome(unsigned int index, RealChromosome &chromosome)
{
    if (index >= mOptions.populationSize) {
        cerr << "ERROR: popInitSetChromosome: index " << index << " is out of range [0," << mOptions.populationSize << ")" << endl;
    } else {
        mPopulation[index] = chromosome;
        // TODO : Check the LB and UB
    }
    if(SORT)partial_sort(mPopulation.begin(), mPopulation.begin()+mElitismNumber, mPopulation.end());
}

// TODO
void RealGen::popInitSetPopulation(vector<RealChromosome> &population)
{
    if (mPopulation.size() != mOptions.populationSize) {
        cerr << "ERROR: popInitSetPopulation: size of population " << mPopulation.size() << " is not the same as Np = " << mOptions.populationSize << endl;
    } else {
        mPopulation = population;
        // TODO : Check the LB and UB
        if(SORT)partial_sort(mPopulation.begin(), mPopulation.begin()+mElitismNumber, mPopulation.end());
    }
}

//==================================== Crossover ==================
void RealGen::crossoverUniform(int indexA, int indexB, RealChromosome &c) {
    if (indexA < 0 || indexA >= mOptions.populationSize) {
        cerr << "crossoverUniform error: indexA = " << indexA << endl;
        exit(-1);
    }
    if (indexB < 0 || indexB >= mOptions.populationSize) {
        cerr << "crossoverUniform error: indexB = " << indexB << endl;
        exit(-1);
    }
    for(int j=0; j<mOptions.chromosomeSize; j++) {
        if(Stat::randUniform()<0.5) {
            c.gene[j] = mPopulation[indexA].gene[j];
        } else {
            c.gene[j] = mPopulation[indexB].gene[j];
        }
    }
}


void RealGen::crossoverFixed(int indexA, int indexB, RealChromosome &c, int n) {
    for(int i=0; i<n; i++) {
        c.gene[i] = mPopulation[indexA].gene[i];
    }
    for(int i=n; i<mOptions.chromosomeSize; i++) {
        c.gene[i] = mPopulation[indexB].gene[i];
    }
}



//===================================== Mutation ======================
void RealGen::uniformMutate(RealChromosome &chromosome, float mutationRate, float perc) {
    for (int i=0; i<mOptions.chromosomeSize; i++) {
        if(Stat::randUniform() < mutationRate) {
            chromosome.uniformMutate(i, perc);
        }
    }
}

void RealGen::gaussianMutate(RealChromosome &chromosome, float mutationRate, float perc) {
    for (int j=0; j<mOptions.chromosomeSize; j++) {
        if(Stat::randUniform() < mutationRate) {
            chromosome.gaussianMutate(j, perc);
        }
    }
}
