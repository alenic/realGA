#include "realgen.h"

// Constructors
RealGen::RealGen()
{
    mTourIndex = nullptr;
    // statistics
    mMinFitness = -1;
    mIndexMinFitness = -1;
    mMaxFitness = -1;
    mIndexMaxFitness = -1;
    mFitnessFcn = nullptr;
    mVerbose = false;

}

RealGen::RealGen(RealGenOptions &opt) : RealGen() {
    setOptions(opt);
}

// Destructor
RealGen::~RealGen() {
    if(mTourIndex != nullptr) {
        delete []mTourIndex;
    }
    delete [] mSigma;
}


void RealGen::setOptions(RealGenOptions &opt) {
    mOptions = opt;
    mNp = mOptions.populationSize;
    mNx = mOptions.chromosomeSize;

    if (mNp != mPopulation.size()) {
        mPopulation.resize(mNp, mNx);
        mNewPopulation.resize(mNp, mNx);
    }
    // Set bounds to all chromosomes
    for (int i = 0; i < mPopulation.size(); ++i) {
        mPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
        mNewPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
    }

    // Initialize standars deviations or gaussian mutation
    mSigma = new float[mNx];
    for (int i = 0; i<mNx; i++) {
        mSigma[i] = mOptions.mutationGaussianScale;
    }
    // Tournament indices array
    if (mTourIndex != nullptr) {
        delete[]mTourIndex;
    }
    mTourIndex = new int[mOptions.selectionTournamentP];
    mMaxGenerations = mOptions.maxGenerations;

    if (mVerbose) {
        checkOptions();
    }
}

void RealGen::checkOptions() {
    cout << "------ checkOptions ------" << endl << endl;
    switch (mOptions.selectionType) {
    case ROULETTE_WHEEL_SELECTION:
        cout << "-> Selection: ROULETTE_WHEEL_SELECTION" << endl;
        break;
    case TOURNAMENT_SELECTION:
        if(mOptions.selectionTournamentP >= 2 && mOptions.selectionTournamentP <= mNp) {
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

    if (mOptions.selectionSorting) {
        cout << "-> Population Sorting : true" << endl;
    } else {
        cout << "-> Population Sorting : false" << endl;
    }

    cout << endl;

    switch (mOptions.crossoverType) {
    case UNIFORM_CROSSOVER:
        cout << "-> Crossover: UNIFORM_CROSSOVER" << endl;
        break;
    case SINGLE_POINT_CROSSOVER:
        if(mOptions.crossoverIndex1 >= 0 && mOptions.crossoverIndex1 < mNx) {
            cout << "-> Crossover: SINGLE_POINT_CROSSOVER : index1 = " << mOptions.crossoverIndex1 << endl;
        } else {
            cerr << "ERROR: options.crossover.index1 must be a number between 0 and Nx" << endl;
            exit(-1);
        }
        break;
    case TWO_POINT_CROSSOVER:
        if(mOptions.crossoverIndex1 >= 0 && mOptions.crossoverIndex1 < mNx &&
                mOptions.crossoverIndex2 >= 0 && mOptions.crossoverIndex2 < mNx) {
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
void RealGen::setFitnessFunction(FitnessFunction *f) {
    mFitnessFcn = f;
}

void RealGen::setSorting(bool value) {
    mOptions.selectionSorting = value;
}


void RealGen::setPopulationSize(int n) {
    mNp = n;
    mPopulation.clear();
    mNewPopulation.clear();
    if(mNp != mPopulation.size()) {
        mPopulation.resize(mNp,mNx);
    }
}

void RealGen::setMutationRate(float value) {
    if(value >= 0 && value <= 1) {
        mOptions.mutationRate = value;
    } else {
        cerr << "ERROR: options.mutation.mutationRate must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }
}

void RealGen::setElitismFactor(float value) {
    if(value >= 0.0 && value <= 1.0) {
        mOptions.selectionElitismFactor = value;
    } else {
        cerr << "ERROR: options.selection.elitismFactor must be a number between 0.0 and 1.0" << endl;
        exit(-1);
    }
}

void RealGen::setMaxGenerations(int maxG) {
    mMaxGenerations = maxG;
}


void RealGen::setSeed(unsigned int seed) {
    Stat::setSeed(seed);
}

void RealGen::setVerbose(bool value)
{
    mVerbose = value;
}

void RealGen::setPopulation(int index, RealGenotype &chromosome) {
    if (index >= mNp) {
        cerr << "ERROR: setPopulation: index " << index << " is out of range [0," << mNp << "]" << endl;
    } else {
        mPopulation[index] = chromosome;
    }
}

void RealGen::setPopulation(vector<RealGenotype>  &new_population) {
    if (mPopulation.size() != mNp) {
        cerr << "ERROR: setPopulation: size of population " << mPopulation.size() << " is not the same as Np = " << mNp << endl;
    } else {
        mPopulation = new_population;
    }
}

// ========================================= Getter ======================================

int RealGen::getGeneration() {
    return mGeneration;
}


double RealGen::evalFitness(const RealGenotype &x) {
    return mFitnessFcn->eval(x);
}

double RealGen::getMeanFitness() {
    double meanF = 0.0;
    for(int i=0; i<mNp; i++) {
        meanF += mPopulation[i].fitness;
    }
    return meanF / (double)mNp;
}

RealGenotype RealGen::getBestChromosome() {
    RealGenotype best;
    best = mPopulation[mIndexMinFitness];
    return best;
}

double RealGen::getMinFitness() {
    return mMinFitness;
}

double RealGen::getDiversity() {  // TO Test
    double I = 0.0;
    // Compute the centroid
    for(int j=0; j<mNx; j++) {
        double ci = 0.0;
        for(int i=0; i<mNp; i++) {
            ci += mPopulation[i].gene[j];
        }
        ci /= mNp;
        for(int i=0; i<mNp; i++) {
            double dx = mPopulation[i].gene[j] - ci;
            I += dx*dx;
        }
    }
    return I;
}

vector<RealGenotype> RealGen::getPopulation() {
    return mPopulation;
}

void RealGen::evalMinFitness(){
    mMinFitness = mPopulation[0].fitness;
    mIndexMinFitness = 0;
    for(int i=1; i<mNp; i++) {
        if(mPopulation[i].fitness < mMinFitness) {
            mMinFitness = mPopulation[i].fitness;
            mIndexMinFitness = i;
        }
    }
}

void RealGen::evalMaxFitness(){
    mMaxFitness = mPopulation[0].fitness;
    mIndexMaxFitness = 0;
    for(int i=1; i<mNp; i++) {
        if(mPopulation[i].fitness > mMaxFitness) {
            mMaxFitness = mPopulation[i].fitness;
            mIndexMaxFitness = i;
        }
    }
}

string RealGen::populationToString() {
    std::ostringstream os;
    RealGenotype x;
    os << "============== Generation: " << mGeneration << " ===================" << endl;
    for(int i=0; i<mNp; i++) {
        x = mPopulation[i];
        os << "[" << (i+1) << "] : "<< x.toString() << " -> Fitness " << x.fitness << endl;
    }
    return os.str();
}


bool RealGen::checkChromosome(RealGenotype &chromosome) {
    for (int j = 0; j < mNx; ++j) {
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
    for (int i = 0; i < mNp; i++) {
        if (!checkChromosome(mPopulation[i])) {
            cout << "checkPopulation Failed: Chromosome " << i << endl;
            exit(-1);
        }
    }
}


// ==================================================== Evolve ====================================================
void RealGen::evolve() {
    RealGenotype offspring(mNx);
    offspring.setBounds(mOptions.lowerBounds, mOptions.upperBounds);

    int index1 = 0, index2 = 1;
    int elitismIndex = (int)(mOptions.selectionElitismFactor*mNp);
    int k=0;

    if(mOptions.selectionType == ROULETTE_WHEEL_SELECTION) {
        sumFitnessRoulette();
    }

    if(mOptions.selectionSorting) {
        while (k < elitismIndex) {
            mNewPopulation[k] = mPopulation[k];
            ++k;
        }
    } else {
        // Keep only the best one
        mNewPopulation[k] = mPopulation[mIndexMinFitness];
        ++k;
    }

    while(k < mNp) {
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
        crossoverUniform(index1, index2, offspring);

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

    if(mOptions.selectionSorting) {
        partial_sort(mNewPopulation.begin(), mNewPopulation.begin()+elitismIndex, mNewPopulation.end());
    }

    if(mOptions.mutationType == GAUSSIAN_MUTATION) {
        for(int i=0; i<mNx; i++) {
            if (mGeneration >= mMaxGenerations) {
                mSigma[i] = mOptions.mutationGaussianScale*(1.0 - mOptions.mutationGaussianShrink);
            } else {
                mSigma[i] = mOptions.mutationGaussianScale*(1.0 - mOptions.mutationGaussianShrink*((float)mGeneration/(float)mMaxGenerations));
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
    mGeneration=0;
    for(int i=0; i<mNp; i++) {
        mPopulation[i].uniformRandom();
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    if(mOptions.selectionSorting) {
        sort(mPopulation.begin(), mPopulation.end());
    }

    // evaluate statistics
    evalMinFitness();
    evalMaxFitness();
}


void RealGen::initMutate(vector<float> &gene, float sigma) {
    mGeneration=0;
    RealGenotype g(mNx);
    g.setBounds(mOptions.lowerBounds, mOptions.upperBounds);
    for(int i=0; i<mNx; i++) {
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
    for(int i=1; i<mNp; i++) {
        mPopulation[i] = g;
        for(int j=0; j<mNx; ++j) {
            mPopulation[i].gaussianLocalRandom(j, sigma);
        }
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    if(mOptions.selectionSorting) {
        sort(mPopulation.begin(), mPopulation.end());
    }

    // evaluate statistics
    evalMinFitness();
    evalMaxFitness();
}


void RealGen::evalPopulationFitness() {
    mGeneration=0;
    for(int i=0; i<mNp; i++) {
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    if(mOptions.selectionSorting) {
        //sort(population.begin(), population.end());
        partial_sort(mPopulation.begin(), mPopulation.begin()+int(mOptions.selectionElitismFactor*mNp), mPopulation.end());
    }
}

// ================= Selection =================================
void RealGen::rouletteWheelSelection(int &index1, int &index2) {
    rouletteWheel(index1, mSumFitnessR*Stat::uniformRand());
    rouletteWheel(index2, mSumFitnessR*Stat::uniformRand());
    if(index1 == index2) {
        if(index1 != mNp-1)
            index2 = index1 + 1;
        else
            index2 = index1 - 1;
    }
}

void RealGen::sumFitnessRoulette() {
    double s=0.0;
    for(int i=0; i<mNp; i++) {
        s += mPopulation[i].fitness;
    }
    mSumFitnessR = mNp*mMaxFitness-s;
}

void RealGen::rouletteWheel(int &index, float stop) {
    float sumP = 0.0;
    index = 0;
    for(int i=0; i<mNp; i++) {
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
        if(index1 != mNp-1)
            index2 = index1 + 1;
        else
            index2 = index1 - 1;
    }
}

void RealGen::tournamentSelect(int p, int &iMin) {
    iMin = 0;
    double fMin = mPopulation[0].fitness;
    for(int i=0; i<p; i++) {
        mTourIndex[i] = Stat::uniformIndex(mNp);
    }
    for(int i=0; i<p; i++) {
        double f=mPopulation[mTourIndex[i]].fitness;
        if(f < fMin) {
            fMin = f;
            iMin = mTourIndex[i];
        }
    }
}
//==================================== Crossover ==================
void RealGen::crossoverUniform(int index1, int index2, RealGenotype &c) {
    if (index1 < 0 || index1 >= mNp) {
        cerr << "crossoverUniform error: index1 = " << index1 << endl;
        exit(-1);
    }
    if (index2 < 0 || index2 >= mNp) {
        cerr << "crossoverUniform error: index2 = " << index2 << endl;
        exit(-1);
    }
    for(int j=0; j<mNx; j++) {
        if(Stat::uniformRand()<0.5) {
            c.gene[j] = mPopulation[index1].gene[j];
        } else {
            c.gene[j] = mPopulation[index2].gene[j];
        }
    }
}

void RealGen::crossoverFixed(int index1, int index2, RealGenotype &c, int n) {
    for(int i=0; i<n; i++) {
        c.gene[i] = mPopulation[index1].gene[i];
    }
    for(int i=n; i<mNx; i++) {
        c.gene[i] = mPopulation[index2].gene[i];
    }
}
//===================================== Mutation ======================
void RealGen::uniformMutate(RealGenotype &g, float perc) {
    for (int i=0; i<mNx; i++) {
        if(Stat::uniformRand() < mOptions.mutationRate) {
            g.uniformLocalRandom(i, perc);
        }
    }
}

void RealGen::gaussianMutate(RealGenotype &g) {
    for (int j=0; j<mNx; j++) {
        if(Stat::uniformRand() < mOptions.mutationRate) {
            g.gaussianLocalRandom(j, mSigma[j]);
        }
    }
}
