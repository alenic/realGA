#include "realga.h"
#include <fstream>
#include <iostream>
#include <unordered_set>

RealGA::RealGA()
{
    // Reset pointers and fitness values
    mFitnessFcn = nullptr;
    mGeneration = 0;
    mSelectionAlgorithm = nullptr;
    mCrossover = nullptr;
    mMutation = nullptr;
    mGaussianMutationPerc = 1.0f;
    mUniformMutationPerc = 1.0f;
}

RealGA::~RealGA()
{
    if (mCrossover != nullptr)
    {
        delete mCrossover;
        mCrossover = nullptr;
    }

    if (mSelectionAlgorithm != nullptr)
    {
        delete mSelectionAlgorithm;
        mSelectionAlgorithm = nullptr;
    }

    if (mMutation != nullptr)
    {
        delete mMutation;
        mMutation = nullptr;
    }
}

// ========================================= Setter ======================================

void RealGA::resetPopulation()
{
    // Check if populationSize has changed
    if (mOptions.populationSize != mPopulation.size())
    {
        mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
        mFitnessValues.resize(mOptions.populationSize);
    }
    // Check if chromosomeSize has changed
    if (mPopulation.size() != 0)
    {
        if (mOptions.chromosomeSize != mPopulation[0].gene.size())
        {
            mPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mNewPopulation.resize(mOptions.populationSize, mOptions.chromosomeSize);
            mFitnessValues.resize(mOptions.populationSize);
        }
    }
}

void RealGA::resetMutationPerc()
// Initialize standars deviations for gaussian mutation
{
    mGaussianMutationPerc = 1.0f;
    mUniformMutationPerc = 1.0f;
}

void RealGA::restart()
{
    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        resetMutationPerc();
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

    if (mSelectionAlgorithm) { delete mSelectionAlgorithm; mSelectionAlgorithm = nullptr; }
    if (mCrossover) { delete mCrossover; mCrossover = nullptr; }
    if (mMutation) { delete mMutation; mMutation = nullptr; }


    if (keepState)
    {
        REALGA_ERROR(mPopulation.empty(), "reset is false, but chromosome population size is 0");
        REALGA_ERROR(mOptions.populationSize != mPopulation.size(), "reset is false, but population size " << mPopulation.size() << " is different from option population size " << mOptions.populationSize);
        REALGA_ERROR(mOptions.chromosomeSize != mPopulation[0].gene.size(), "reset is false, but chromosome size " << mPopulation[0].gene.size() << " is different from option chromosome size " << mOptions.chromosomeSize);
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
            resetMutationPerc();
        }
            // Always clean up old strategies (move outside if-block)

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
        case LINEAR_CROSSOVER:
            mCrossover = new LinearCrossover(mOptions.chromosomeSize);
            break;
        }
        REALGA_ERROR(mCrossover == nullptr, "Crossover is null");
        // Create the mutation strategy
        switch (mOptions.mutationType)
        {
        case UNIFORM_MUTATION:
            mMutation = new UniformMutation(mOptions.mutationRate, mUniformMutationPerc);
            break;
        case GAUSSIAN_MUTATION:
            mMutation = new GaussianMutation(mOptions.mutationRate, mGaussianMutationPerc);
            break;
        }

        Stat::setSeed(mOptions.seed);
        mGeneration = 0;
        mOptions.checkOptions();
        // Set bounds
        mLB = mOptions.lowerBounds;
        mUB = mOptions.upperBounds;
    }
}

void RealGA::setFitnessFunction(FitnessFunction *f)
{
    mFitnessFcn = f;
}

void RealGA::fillFitnessValues(vector<RealChromosome> &population)
{
    for (size_t i = 0; i < population.size(); i++)
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
    for (size_t i = 1; i < mPopulation.size(); i++)
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

const vector<RealChromosome> &RealGA::getPopulation() const
{
    return mPopulation;
}

string RealGA::populationToString()
{
    std::ostringstream os;
    os << "Population of Generation: " << mGeneration << "\n";
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        os << i << ": " << mPopulation[i].toString() << " -> Fitness " << mPopulation[i].fitness << "\n";
    }
    return os.str();
}

string RealGA::populationToCSVString()
{
    std::ostringstream os;
    os << "id,";
    for (size_t i = 0; i < mPopulation[i].gene.size(); i++)
    {
        os << "x" << i << ",";
    }
    os << "fitness\n";
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        string geneString = mPopulation[i].toString();
        os << i << "," << geneString.substr(1, geneString.size() - 2) << "," << mPopulation[i].fitness << "\n";
    }
    return os.str();
}

void RealGA::populationToCSV(const std::string &csvFilename)
{
    // Write the current population to a CSV file
    std::ofstream ofs(csvFilename);
    if (!ofs.is_open())
    {
        std::cerr << "Error: Could not open file " << csvFilename << " for writing." << std::endl;
        return;
    }

    // Write header
    ofs << populationToCSVString();
    ofs.close();
}

void RealGA::checkPopulation()
{
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        for (size_t j = 0; j < mOptions.chromosomeSize; ++j)
        {
            REALGA_ERROR(isnan(mPopulation[i].gene[j]) || isinf(mPopulation[i].gene[j]),
                         "error in chromosome " << i << " " << mPopulation[i].toString());
        }
    }
}

// ==================================================== Evolve ====================================================
void RealGA::evolve()
{
    int n = mOptions.populationSize;
    int elitismNumber = (int)(mOptions.elitismFactor * n);

    // 1. Efficiency: Use nth_element to move the best individuals to the front
    // This is O(N) on average and handles ties perfectly.
    std::nth_element(mPopulation.begin(), 
                     mPopulation.begin() + elitismNumber, 
                     mPopulation.end(), 
                     [](const RealChromosome& a, const RealChromosome& b) {
                         return a.fitness < b.fitness;
                     });

    // 2. Use a Hash Set for O(1) duplicate lookups
    // We store fitness values as 'long long' or use a custom epsilon-based hash
    // for floating point safety, but a set of floats is the simplest upgrade.
    std::unordered_set<float> fitnessRegistry;
    int newPopIdx = 0;

    // 3. Transfer Elites
    for (; newPopIdx < elitismNumber; ++newPopIdx)
    {
        mNewPopulation[newPopIdx] = mPopulation[newPopIdx];
        if (mOptions.mutateDuplicatedFitness) {
            fitnessRegistry.insert(mNewPopulation[newPopIdx].fitness);
        }
    }

    // 4. Generate Offspring
    RealChromosome offspring(mOptions.chromosomeSize);
    while (newPopIdx < n)
    {
        int idxA, idxB;
        mSelectionAlgorithm->select(mFitnessValues, idxA, idxB);
        
        mCrossover->crossover(mPopulation[idxA], mPopulation[idxB], offspring);
        mMutation->mutate(offspring, mLB, mUB);
        offspring.fitness = evalFitness(offspring);

        // O(1) Duplicate Check
        if (mOptions.mutateDuplicatedFitness)
        {
            int attempts = 0;
            // If duplicate found, mutate again (limit attempts to avoid infinite loops)
            while (fitnessRegistry.find(offspring.fitness) != fitnessRegistry.end() && attempts < 10)
            {
                mMutation->mutate(offspring, mLB, mUB);
                offspring.fitness = evalFitness(offspring);
                attempts++;
            }
            fitnessRegistry.insert(offspring.fitness);
        }

        mNewPopulation[newPopIdx] = offspring;
        newPopIdx++;
    }

    // 5. Update mutation cooling parameters

    if (mOptions.mutationType == GAUSSIAN_MUTATION)
    {
        mGaussianMutationPerc = fmax(mOptions.mutationGaussianPercMin, 1.0f - mOptions.mutationGaussianPercDelta * (float)mGeneration);
        mMutation->setMutationPercentage(mGaussianMutationPerc);
    }

    if (mOptions.mutationType == UNIFORM_MUTATION)
    {
        mUniformMutationPerc = fmax(mOptions.mutationUniformPercMin, 1.0f - mOptions.mutationUniformPercDelta * (float)mGeneration);
        mMutation->setMutationPercentage(mUniformMutationPerc);
    }

    mPopulation = mNewPopulation;
    
    // Crucial: Update the fitness value cache for the Selection Algorithm next turn
    fillFitnessValues(mPopulation); 
    
    mGeneration++;
}
// ===================================== Init function =============================
void RealGA::popInitRandUniform()
// Init random uniform population
{
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].randUniform(mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    fillFitnessValues(mPopulation);
}

void RealGA::popInitRandGaussian(float mean, float sigma)
// Init random gaussian population
{
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].randGaussian(mean, sigma, mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    fillFitnessValues(mPopulation);
}

void RealGA::popInitGaussianMutate(vector<float> &gene, float mutatioRate, float mutationPerc)
// Init population from a given chromosome and mutate it
{
    RealChromosome g(mOptions.chromosomeSize);
    GaussianMutation mutation(mutatioRate, mutationPerc);
    for (size_t i = 0; i < mOptions.chromosomeSize; i++)
    {
        g.gene[i] = gene[i];
    }
    mPopulation[0] = g;
    mPopulation[0].fitness = evalFitness(mPopulation[0]);
    for (size_t i = 1; i < mOptions.populationSize; i++)
    {
        mPopulation[i] = g;
        mutation.mutate(mPopulation[i], mLB, mUB);
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
    fillFitnessValues(mPopulation);
}

// TODO
void RealGA::popInitSetChromosome(unsigned int index, RealChromosome &chromosome)
{
    REALGA_ERROR(index >= mOptions.populationSize, "index " << index << " is out of range [0," << mOptions.populationSize << ")");
    // Check LB and UB
    for (size_t i = 0; i < chromosome.gene.size(); i++)
        REALGA_ERROR(chromosome.gene[i] < mLB[i] || chromosome.gene[i] > mUB[i], "chromosome " << chromosome.toString() << " gene " << i << " is " << chromosome.gene[i]);
    mPopulation[index] = chromosome;
    mPopulation[index].fitness = evalFitness(mPopulation[index]);
}

// TODO
void RealGA::popInitSetPopulation(vector<RealChromosome> &population)
{
    REALGA_ERROR(mPopulation.size() != mOptions.populationSize, "size of population " << mPopulation.size() << " is not the same as Np = " << mOptions.populationSize);
    // TODO: Check chromosomes LB and UB
    mPopulation = population;
}

void RealGA::evaluatePopulationFitness()
{
    for (size_t i = 0; i < mOptions.populationSize; i++)
    {
        mPopulation[i].fitness = evalFitness(mPopulation[i]);
    }
}
