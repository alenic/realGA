#include "selection.h"


TournamentSelection::TournamentSelection(int populationSize) {
    mTournamentSize = (int)(populationSize*0.1);
    mPopulationSize = populationSize;
    if (mTournamentSize == 0) mTournamentSize = 1;
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
    if((selectionProbability <= 0.0) || (selectionProbability>1.0)) {
        cerr << "TournamentSelection: selectionProbability must be in (0,1]" << endl;
        exit(-1);
    }
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
    if(indexA == indexB) {
        if(indexA != mPopulationSize-1) {
            indexB = indexA + 1;
        } else {
            indexB = indexA - 1;
        }
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

    int kth = 0;
    int kthMinIndex = 0;
    float minValue;
    
    if(mSelectionProbability < 1.0) {
        float chooseP = mSelectionProbability;
        // choose the winner with a probability p, 2nd with p*(1-p), 3rd with p*(1-p)*2,...
        for(int j=0; j<mTournamentSize; j++) {
            if(Stat::randUniform() < chooseP) {
                kth = j;
                break;
            }
            chooseP *= (1.0 - mSelectionProbability);
        }
        int minIndex;
        for(int k=0; k<kth; k++) {
            minValue = mTournamentFitness[0];
            minIndex = 0;
            for(int i=1; i<mTournamentSize; i++) {
                if(minValue < mTournamentFitness[i]) {
                    minValue = mTournamentFitness[i];
                    minIndex = i;
                }
            }
            mTournamentFitness[minIndex] = MAXFLOAT;
        }
    }

    // get the min
    minValue = mTournamentFitness[0];
    kthMinIndex = 0;
    for(int i=1; i<mTournamentSize; i++) {
        if(minValue < mTournamentFitness[i]) {
            minValue = mTournamentFitness[i];
            kthMinIndex = i;
        }
    }

    return kthMinIndex;
}
