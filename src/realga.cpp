#include "realga.h"

RealGA::RealGA()
{
    // Reset pointers and fitness values
    mFitnessFcn = nullptr;
    mGeneration = 0;
    mSelectionAlgorithm = nullptr;
    mCrossover = nullptr;
    mMutation = nullptr;
    mGaussianPerc = 1.0f;
}

RealGA::~RealGA()
{
    if (mCrossover != nullptr)
        delete mCrossover;
    if (mSelectionAlgorithm != nullptr)
        delete mSelectionAlgorithm;
    if (mMutation != nullptr)
        delete mMutation;
}

// ========================================= Setter ======================================

void RealGA::resetPopulation()
{
    // Check if populationSize has changed
    if (mOptions.populationSize != mPopulation.size())
    {
        mPopulation.clear();
        mNewPopulation.clear();
        mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mFitnessValues.resize(mOptions.populationSize);
    }
    // Check if chromosomeSize has changed
    if (mPopulation.size() != 0)
    {
        if (mOptions.chromosomeSize != mPopulation[0].gene.size())
        {
            mPopulation.clear();
            mNewPopulation.clear();
            mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mFitnessValues.resize(mOptions.populationSize);
        }
    }
}

void RealGA::resetGaussianMutationPerc()
// Initialize standars deviations for gaussian mutation
{
    mGaussianPerc = 1.0f;
}

void RealGA::restart()
{
    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        resetGaussianMutationPerc();
    }
    mGeneration = 0;
}

void RealGA::init(RealGAOptions &opt, FitnessFunction *func, bool keepState)
// Set custom options
{
    // set options
    mOptions = opt;
    // set fitness function
    mFitnessFcn = func;

    if (keepState)
    {
        if (mPopulation.size() == 0)
        {
            cerr << "ERROR: setOptions: reset is false, but chromosome population size is 0" << endl;
            exit(-1);
        }
        else if (mOptions.populationSize != mPopulation.size())
        {
            cerr << "ERROR: setOptions: reset is false, but population size " << mPopulation.size()
                 << " is different from option population size " << mOptions.populationSize << endl;
            exit(-1);
        }

        if (mOptions.chromosomeSize != mPopulation[0].gene.size())
        {
            cerr << "ERROR: setOptions: reset is false, but chromosome size " << mPopulation[0].gene.size()
                 << " is different from option chromosome size " << mOptions.chromosomeSize << endl;
            exit(-1);
        }
    }
    else
    {
        // reset population size
        resetPopulation();

        // Set bounds to all chromosomes
        // for (int i = 0; i < mPopulation.size(); ++i) {
        //    mPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
        //    mNewPopulation[i].setBounds(mOptions.lowerBounds, mOptions.upperBounds);
        //}

        // Initialize gaussian standard deviation
        if (mOptions.mutationType == GAUSSIAN_MUTATION)
        {
            resetGaussianMutationPerc();
        }

        // Create the selection strategy
        switch (mOptions.selectionType)
        {
        case ROULETTE_WHEEL_SELECTION:
            mSelectionAlgorithm = new RouletteWheelSelection(mOptions.populationSize);
            break;
        case TOURNAMENT_SELECTION:
            mSelectionAlgorithm = new TournamentSelection(mOptions.populationSize);
            ((TournamentSelection *)mSelectionAlgorithm)->setTournamentSize(mOptions.selectionTournamentSize);
            ((TournamentSelection *)mSelectionAlgorithm)->setSelectionProbability(mOptions.selectionTournamentProbability);
            break;
        }
        REALGA_ERROR(mSelectionAlgorithm == nullptr, "SelectionAlgorithm is null");
        // Create the crossover strategy
        switch (mOptions.crossoverType)
        {
        case UNIFORM_CROSSOVER:
            mCrossover = new UniformCrossover(mOptions.chromosomeSize);
            break;
        }
        REALGA_ERROR(mCrossover == nullptr, "Crossover is null");
        // Create the mutation strategy
        switch (mOptions.mutationType)
        {
        case UNIFORM_MUTATION:
            mMutation = new UniformMutation(mOptions.mutationRate, mOptions.mutationUniformPerc);
            break;
        case GAUSSIAN_MUTATION:
            mMutation = new GaussianMutation(mOptions.mutationRate, mGaussianPerc);
            break;
        }

        Stat::setSeed(mOptions.seed);
        mGeneration = 0;
        mOptions.checkOptions();
        // Set bounds
        mLB = mOptions.lowerBounds;
        mUB = mOptions.upperBounds;
    }
    mElitismNumber = (int)(mOptions.elitismFactor * mOptions.populationSize);
}

void RealGA::setFitnessFunction(FitnessFunction *f)
{
    mFitnessFcn = f;
}

void RealGA::fillFitnessValues(vector<RealChromosome> &population)
{
    for (int i = 0; i < population.size(); i++)
    {
        mFitnessValues[i] = population[i].fitness;
    }
}
// ========================================= Getter ======================================

int RealGA::getGeneration()
{
    return mGeneration;
}

float RealGA::evalFitness(const RealChromosome &x)
{
    float eval = mFitnessFcn->eval(x);
    if (isnan(eval) || isinf(eval))
    {
        return MAXFLOAT;
    }
    return eval;
}

RealChromosome RealGA::getBestChromosome()
{
    int minIndex = 0;
    float minValue = mPopulation[0].fitness;
    for (int i = 1; i < mPopulation.size(); i++)
    {
        float value = mPopulation[i].fitness;
        if (value < minValue)
        {
            minValue = value;
            minIndex = i;
        }
    }
    return mPopulation[minIndex];
}

/*
float RealGA::getDiversity() {  //  TODO
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

vector<RealChromosome> RealGA::getPopulation()
{
    return mPopulation;
}

string RealGA::populationToString()
{
    std::ostringstream os;
    RealChromosome x;
    os << "============== Generation: " << mGeneration << " ===================" << endl;
    for (int i = 0; i < mOptions.populationSize; i++)
    {
        x = mPopulation[i];
        os << "[" << (i + 1) << "] : " << x.toString() << " -> Fitness " << x.fitness << endl;
    }
    return os.str();
}

void RealGA::checkPopulation()
{
    for (int i = 0; i < mOptions.populationSize; i++)
    {
        for (int j = 0; j < mOptions.chromosomeSize; ++j)
        {
            REALGA_ERROR(isnan(mPopulation[i].gene[j]) || isinf(mPopulation[i].gene[j]),
                         "error in chromosome " << i << " " << mPopulation[i].toString());
        }
    }
}

// ==================================================== Evolve ====================================================
void RealGA::evolve()
{
    // Allocate offspring (gene after crossover and mutation)
    RealChromosome offspring(mOptions.chromosomeSize);
    int selectedIndexA, selectedIndexB;
    int k = 0;
    int k_e = 0;
    int countElite = 0;

    fillFitnessValues(mPopulation);
    // Find the kth smallest Fitness (WARNING it modify mFitnessValues!)
    mKthSmallestFitness = RALG::kthSmallest(mFitnessValues, 0, mOptions.populationSize - 1, mElitismNumber + 1);
    // fill mFitnessValues to accelerate some functions
    fillFitnessValues(mPopulation);
    // Keep first mElitismNumber population elitism
    while (k_e < mOptions.populationSize)
    {
        if ((mFitnessValues[k_e] < mKthSmallestFitness) && (k <= mElitismNumber))
        {
            mNewPopulation[k] = mPopulation[k_e];
            // cout << mKthSmallestFitness << " -> mNewPopulation[" << k << "] = " << mFitnessValues[k_e] << "    " << mPopulation[k_e].fitness << endl;
            ++k;
        }
        ++k_e;
    }

    // Generate New Population
    while (k < mOptions.populationSize)
    {
        // Selection
        mSelectionAlgorithm->select(mFitnessValues, selectedIndexA, selectedIndexB);
        // Crossover
        mCrossover->crossover(mPopulation[selectedIndexA], mPopulation[selectedIndexB], offspring);
        // Mutation
        if (mOptions.mutationType == GAUSSIAN_MUTATION)
            mMutation->setMutationPercentage(mGaussianPerc);
        mMutation->mutate(offspring, mLB, mUB);

        offspring.fitness = evalFitness(offspring);

        if (mOptions.mutateDuplicatedFitness)
        {
            // Force mutation if a chromosome fitness is duplicated
            for (int j = 0; j < k; j++)
            {
                if (offspring.fitness == mNewPopulation[j].fitness)
                {
                    mMutation->mutate(offspring, mLB, mUB);
                    offspring.fitness = evalFitness(offspring);
                }
            }
        }

        mNewPopulation[k] = offspring;
        ++k;
    }

    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        if (mGaussianPerc > mOptions.mutationGaussianPercMin)
        {
            mGaussianPerc = 1.0f - mOptions.mutationGaussianPercDelta * (float)mGeneration;
        }
        else
        {
            mGaussianPerc = mOptions.mutationGaussianPercMin;
        }
    }

    // Copy the new population
    mPopulation = mNewPopulation;
    mGeneration++;
}

// ===================================== Init function =============================
void RealGA::popInitRandUniform()
// Init random uniform population
{
    for (int i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].randUniform(mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
}

void RealGA::popInitRandGaussian(float mean, float sigma)
// Init random gaussian population
{
    for (int i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].randGaussian(mean, sigma, mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
}

void RealGA::popInitGaussianMutate(vector<float> &gene, float mutatioRate, float mutationPerc)
// Init population from a given chromosome and mutate it
{
    RealChromosome g(mOptions.chromosomeSize);
    GaussianMutation mutation(mutatioRate, mutationPerc);
    for (int i = 0; i < mOptions.chromosomeSize; i++)
    {
        g.gene[i] = gene[i];
    }
    mPopulation[0] = g;
    mPopulation[0].fitness = evalFitness(mPopulation[0]);
    for (int i = 1; i < mOptions.populationSize; i++)
    {
        mPopulation[i] = g;
        mutation.mutate(mPopulation[i], mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
}

// TODO
void RealGA::popInitSetChromosome(unsigned int index, RealChromosome &chromosome)
{
    if (index >= mOptions.populationSize)
    {
        cerr << "ERROR: popInitSetChromosome: index " << index << " is out of range [0," << mOptions.populationSize << ")" << endl;
    }
    else
    {
        mPopulation[index] = chromosome;
        mPopulation[index].fitness = evalFitness(mPopulation[index]);
        // TODO : Check the LB and UB
    }
}

// TODO
void RealGA::popInitSetPopulation(vector<RealChromosome> &population)
{
    if (mPopulation.size() != mOptions.populationSize)
    {
        cerr << "ERROR: popInitSetPopulation: size of population " << mPopulation.size() << " is not the same as Np = " << mOptions.populationSize << endl;
    }
    else
    {
        mPopulation = population;
        // TODO : Check the LB and UB
    }
}

void RealGA::evaluatePopulationFitness()
{
    for (int i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
}
