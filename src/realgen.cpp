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
    mTourIndex = nullptr;
    mIndexMinFitness = 0;
    mMaxFitness = -1;
    mIndexMaxFitness = 0;
    mFitnessFcn = nullptr;
    mGeneration = 0;
}

RealGen::~RealGen()
// Destructor
{
    if(mTourIndex != nullptr) {
        delete []mTourIndex;
    }
    if (mOptions.mutationType == GAUSSIAN_MUTATION) {
        delete [] mSigma;
    }
    
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
        if(mOptions.selectionTournamentP >= 2 && mOptions.selectionTournamentP <= mOptions.populationSize) {
            cout << "-> Selection: TOURNAMENT_SELECTION : tournamentP = " << mOptions.selectionTournamentP << endl;
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
        if(mOptions.crossoverIndex1 >= 0 && mOptions.crossoverIndex1 < mOptions.chromosomeSize) {
            cout << "-> Crossover: SINGLE_POINT_CROSSOVER : index1 = " << mOptions.crossoverIndex1 << endl;
        } else {
            cerr << "ERROR: options.crossover.index1 must be a number between 0 and Nx" << endl;
            exit(-1);
        }
        break;
    case TWO_POINT_CROSSOVER:
        if(mOptions.crossoverIndex1 >= 0 && mOptions.crossoverIndex1 < mOptions.chromosomeSize &&
                mOptions.crossoverIndex2 >= 0 && mOptions.crossoverIndex2 < mOptions.chromosomeSize) {
            cout << "-> Crossover: TWO_POINT_CROSSOVER : index1 = " << mOptions.crossoverIndex1 << "index2 = " << mOptions.crossoverIndex2 << endl;
        } else {
            cerr << "ERROR: crossover_index1 and crossover_index2 must be a number between 0 and Nx" << endl;
            exit(-1);
        }
        if(mOptions.crossoverIndex1 > mOptions.crossoverIndex2) {
            cerr << "ERROR: crossover_index1 <= crossover_index2 must be satisfied" << endl;
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
        cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianScale = " << mOptions.mutationGaussianScale << endl;
        cout << "-> Mutation: GAUSSIAN_MUTATION : gaussianShrink = " << mOptions.mutationGaussianShrink << endl;
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
    }
    // Check if chromosomeSize has changed
    if(mPopulation.size() != 0) {
        if(mOptions.chromosomeSize != mPopulation[0].gene.size()) {
            mPopulation.clear();
            mNewPopulation.clear();
            mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        }
    }
}

void RealGen::resetGaussianMutationSigma()
{
    // Initialize standars deviations for gaussian mutation
    mSigma = new float[mOptions.chromosomeSize];
    for (int i = 0; i<mOptions.chromosomeSize; i++) {
        mSigma[i] = mOptions.mutationGaussianScale;
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

        if (mOptions.mutationGaussianScale != opt.mutationGaussianScale) {
            resetGaussianMutationSigma();
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
            resetGaussianMutationSigma();
        }
        
        // Reset tournament selection
        if (mOptions.selectionType == TOURNAMENT_SELECTION) {
            if (mTourIndex != nullptr) {
                delete []mTourIndex;
                mTourIndex = new int[mOptions.selectionTournamentP];
            }
        }

        if (mOptions.verbose) {
            checkOptions();
        }

        Stat::setSeed(mOptions.seed);

        mMinFitness = -1;
        mIndexMinFitness = 0;
        mMaxFitness = -1;
        mIndexMaxFitness = 0;
        mGeneration = 0;
    }



}

void RealGen::setFitnessFunction(FitnessFunction *f) 
{
    mFitnessFcn = f;
}

void RealGen::setChromosomeInPopulation(unsigned int index, RealChromosome &chromosome)
{
    if (index >= mOptions.populationSize) {
        cerr << "ERROR: setPopulation: index " << index << " is out of range [0," << mOptions.populationSize << ")" << endl;
    } else {
        mPopulation[index] = chromosome;
    }
}

void RealGen::setPopulation(vector<RealChromosome> &population)
{
    if (mPopulation.size() != mOptions.populationSize) {
        cerr << "ERROR: setPopulation: size of population " << mPopulation.size() << " is not the same as Np = " << mOptions.populationSize << endl;
    } else {
        mPopulation = population;
    }
}

// ========================================= Getter ======================================

int RealGen::getGeneration() {
    return mGeneration;
}


float RealGen::evalFitness(const RealChromosome &x) {
    return mFitnessFcn->eval(x);
}

float RealGen::getMeanFitness() {
    float meanF = 0.0;
    for(int i=0; i<mOptions.populationSize; i++) {
        meanF += mPopulation[i].fitness;
    }
    return meanF / (float)mOptions.populationSize;
}

RealChromosome RealGen::getBestChromosome() {
    RealChromosome best;
    best = mPopulation[mIndexMinFitness];
    return best;
}

float RealGen::getMinFitness() {
    return mMinFitness;
}

float RealGen::getDiversity() {  // TO Test
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

vector<RealChromosome> RealGen::getPopulation() {
    return mPopulation;
}

void RealGen::evalMinFitness(){
    mMinFitness = mPopulation[0].fitness;
    mIndexMinFitness = 0;
    for(int i=1; i<mOptions.populationSize; i++) {
        if(mPopulation[i].fitness < mMinFitness) {
            mMinFitness = mPopulation[i].fitness;
            mIndexMinFitness = i;
        }
    }
}

void RealGen::evalMaxFitness(){
    mMaxFitness = mPopulation[0].fitness;
    mIndexMaxFitness = 0;
    for(int i=1; i<mOptions.populationSize; i++) {
        if(mPopulation[i].fitness > mMaxFitness) {
            mMaxFitness = mPopulation[i].fitness;
            mIndexMaxFitness = i;
        }
    }
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


// ==================================================== Evolve ====================================================
void RealGen::evolve() {
    RealChromosome offspring(mOptions.chromosomeSize);
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);

    int index1 = 0, index2 = 1;
    int elitismIndex = (int)(mOptions.selectionElitismFactor*mOptions.populationSize);
    int k=0;

    if(mOptions.selectionType == ROULETTE_WHEEL_SELECTION) {
        sumFitnessRoulette();
    }

    while (k < elitismIndex) {
        mNewPopulation[k] = mPopulation[k];
        ++k;
    }

    while(k < mOptions.populationSize) {
        // Selection
        switch(mOptions.selectionType) {
        case ROULETTE_WHEEL_SELECTION:
            rouletteWheelSelection(index1, index2);
            break;
        case TOURNAMENT_SELECTION:
            tournamentSelection(mOptions.selectionTournamentP, index1, index2);
            break;
        }

        // Crossover
        switch(mOptions.crossoverType) {
            case UNIFORM_CROSSOVER:
                crossoverUniform(index1, index2, offspring);
                break;
            case SINGLE_POINT_CROSSOVER:
                crossoverFixed(index1, index2, offspring, mOptions.crossoverIndex1);
                break;
        }
        

        // Mutation
        switch(mOptions.mutationType) {
        case UNIFORM_MUTATION:
            uniformMutate(offspring, mOptions.mutationUniformPerc);
            break;
        case GAUSSIAN_MUTATION:
            gaussianMutate(offspring);
            break;
        }

        offspring.fitness  = evalFitness(offspring);
        mNewPopulation[k] = offspring;
        ++k;
    }

    partial_sort(mNewPopulation.begin(), mNewPopulation.begin()+elitismIndex, mNewPopulation.end());

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        for(int i=0; i<mOptions.chromosomeSize; i++) {
            if (mGeneration >= mOptions.maxGenerations) {
                mSigma[i] = mOptions.mutationGaussianScale*(1.0 - mOptions.mutationGaussianShrink);
            } else {
                mSigma[i] = mOptions.mutationGaussianScale*(1.0 - mOptions.mutationGaussianShrink*((float)mGeneration/(float)mOptions.maxGenerations));
            }
            if (mSigma[i] <= 0) {
                mSigma[i] = 0;
            }
        }
    }

    mPopulation = mNewPopulation;

    evalMinFitness();
    evalMaxFitness();

    mGeneration++;
}

// ===================================== Init function =============================
void RealGen::initRandom() {
    for(int i=0; i<mOptions.populationSize; i++) {
        mPopulation[i].randUniform();
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }

    sort(mPopulation.begin(), mPopulation.end());

    // evaluate statistics
    evalMinFitness();
    evalMaxFitness();
}


void RealGen::initMutate(vector<float> &gene, float sigma) {
    RealChromosome g(mOptions.chromosomeSize);
    g.setBounds(mOptions.lowerBounds, mOptions.upperBounds);
    for(int i=0; i<mOptions.chromosomeSize; i++) {
        if (gene[i] < mOptions.lowerBounds[i]) {
            cerr << "initMutate error, gene[" << i << "]="<< gene[i] <<" violate lowerBounds[" << i << "] = " << mOptions.lowerBounds[i] << endl;
            exit(-1);
        }
        if (gene[i] > mOptions.upperBounds[i]) {
            cerr << "initMutate error, gene[" << i << "]="<< gene[i] <<" violate upperBounds[" << i << "] = " << mOptions.upperBounds[i] << endl;
            exit(-1);
        }
        g.gene[i] = gene[i];
    }
    mPopulation[0] = g;
    mPopulation[0].fitness = evalFitness(mPopulation[0]);
    for(int i=1; i<mOptions.populationSize; i++) {
        mPopulation[i] = g;
        for(int j=0; j<mOptions.chromosomeSize; ++j) {
            mPopulation[i].randGaussianPerc(j, sigma);
        }
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }

    sort(mPopulation.begin(), mPopulation.end());

    // evaluate statistics
    evalMinFitness();
    evalMaxFitness();
}


void RealGen::evalPopulationFitness() {
    for(int i=0; i<mOptions.populationSize; i++) {
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }

    partial_sort(mPopulation.begin(), mPopulation.begin()+int(mOptions.selectionElitismFactor*mOptions.populationSize), mPopulation.end());

}

// ================= Selection =================================
void RealGen::rouletteWheelSelection(int &index1, int &index2) {
    rouletteWheel(index1, mSumFitnessR*Stat::randUniform());
    rouletteWheel(index2, mSumFitnessR*Stat::randUniform());
    if(index1 == index2) {
        if(index1 != mOptions.populationSize-1)
            index2 = index1 + 1;
        else
            index2 = index1 - 1;
    }
}

void RealGen::sumFitnessRoulette() {
    float s=0.0;
    for(int i=0; i<mOptions.populationSize; i++) {
        s += mPopulation[i].fitness;
    }
    mSumFitnessR = mOptions.populationSize*mMaxFitness-s;
}

void RealGen::rouletteWheel(int &index, float stop) {
    float sumP = 0.0;
    index = 0;
    for(int i=0; i<mOptions.populationSize; i++) {
        sumP += (mMaxFitness-mPopulation[i].fitness);
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
        if(index1 != mOptions.populationSize-1)
            index2 = index1 + 1;
        else
            index2 = index1 - 1;
    }
}

void RealGen::tournamentSelect(int p, int &iMin) {
    iMin = 0;
    float fMin = mPopulation[0].fitness;
    for(int i=0; i<p; i++) {
        mTourIndex[i] = Stat::randIndex(mOptions.populationSize);
    }
    for(int i=0; i<p; i++) {
        float f=mPopulation[mTourIndex[i]].fitness;
        if(f < fMin) {
            fMin = f;
            iMin = mTourIndex[i];
        }
    }
}
//==================================== Crossover ==================
void RealGen::crossoverUniform(int index1, int index2, RealChromosome &c) {
    if (index1 < 0 || index1 >= mOptions.populationSize) {
        cerr << "crossoverUniform error: index1 = " << index1 << endl;
        exit(-1);
    }
    if (index2 < 0 || index2 >= mOptions.populationSize) {
        cerr << "crossoverUniform error: index2 = " << index2 << endl;
        exit(-1);
    }
    for(int j=0; j<mOptions.chromosomeSize; j++) {
        if(Stat::randUniform()<0.5) {
            c.gene[j] = mPopulation[index1].gene[j];
        } else {
            c.gene[j] = mPopulation[index2].gene[j];
        }
    }
}

void RealGen::crossoverFixed(int index1, int index2, RealChromosome &c, int n) {
    for(int i=0; i<n; i++) {
        c.gene[i] = mPopulation[index1].gene[i];
    }
    for(int i=n; i<mOptions.chromosomeSize; i++) {
        c.gene[i] = mPopulation[index2].gene[i];
    }
}
//===================================== Mutation ======================
void RealGen::uniformMutate(RealChromosome &g, float perc) {
    for (int i=0; i<mOptions.chromosomeSize; i++) {
        if(Stat::randUniform() < mOptions.mutationRate) {
            g.randUniformPerc(i, perc);
        }
    }
}

void RealGen::gaussianMutate(RealChromosome &g) {
    for (int j=0; j<mOptions.chromosomeSize; j++) {
        if(Stat::randUniform() < mOptions.mutationRate) {
            g.randGaussianPerc(j, mSigma[j]);
        }
    }
}
