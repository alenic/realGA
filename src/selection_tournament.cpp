#include "selection.h"


TournamentSelection::TournamentSelection(int populationSize) {
    mTournamentSize = (int)(populationSize*0.1);
    mPopulationSize = populationSize;
    mSelectionProbability = 0.85;
}

TournamentSelection::~TournamentSelection() {
    if(mTournamentFitness != nullptr) delete []mTournamentFitness;
    if(mTournamentIndex != nullptr) delete []mTournamentIndex;
}

void TournamentSelection::setTournamentSize(int tournamentSize) {
    if (tournamentSize <= 0) {
        cerr << "setTournamentSize: tournamentSize must be >= 1" <<  endl;
        exit(-1);
    }
    mTournamentSize = tournamentSize;
}

void TournamentSelection::setSelectionProbability(float selectionProbability) {
    mSelectionProbability = selectionProbability;
}

void TournamentSelection::init(vector<float> &fitnessValues) {
    mTournamentFitness = new float[mTournamentSize];
    mTournamentIndex = new int[mTournamentSize];
}

void TournamentSelection::select(vector<float> &fitnessValues, int &indexA, int &indexB) {
    // choose randomly mTournamentSize fitness value and get the 1st and 2nd winners
    indexA = tournament(fitnessValues);
    indexB = tournament(fitnessValues);

    while(indexA == indexB) {
        indexA = Stat::randIndex(fitnessValues.size()-1);
        indexB = Stat::randIndex(fitnessValues.size()-1);
    }
}

int TournamentSelection::tournament(vector<float> &fitnessValues) {
    // return the index of winner
    int i=0;
    bool match = false;
    // sample random mTournamentSize indices from mPopulationSize indices, without replacement
    while(i < mTournamentSize) {
        int index = Stat::randIndex(mPopulationSize-1);
        // check if the index was choosen
        match = false;
        for(int j=0; j<i; j++) {
            if(mTournamentIndex[j] == index) {
                match = true;
                break;
            }
        }
        if(!match) {
            mTournamentIndex[i] = index;
            mTournamentFitness[i] = fitnessValues[index];
            i++;
        }
    }

    int kth;
    int kthMinIndex;
    int localKthIndex;
    
    if(mSelectionProbability < 1.0) {
        float chooseP = mSelectionProbability;
        // choose the winner with a probability p, 2nd with p*(1-p), 3rd with p*(1-p)*2,...
        for(kth=1; kth<mTournamentSize; kth++) {
            if(Stat::randUniform() < chooseP) {
                break;
            }
            chooseP *= (1.0 - mSelectionProbability);
        }
        // Get k-th min value
        localKthIndex = RALG::argSelection(mTournamentFitness, 0, mTournamentSize-1, kth);
        kthMinIndex = mTournamentIndex[localKthIndex];
    } else {
        localKthIndex = RALG::argMin(mTournamentFitness, 0, mTournamentSize-1);
        kthMinIndex = mTournamentIndex[localKthIndex];
    }


    return kthMinIndex;
}
