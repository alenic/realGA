#include "selection.h"


TournamentSelection::TournamentSelection(int populationSize) {
    mTournamentSize = (int)(populationSize*0.1);
    mPopulationSize = populationSize;
    if (mTournamentSize == 0) mTournamentSize = 1;
    mSelectionProbability = 0.85;
}

TournamentSelection::~TournamentSelection() {

}

void TournamentSelection::setTournamentSize(int tournamentSize) {
    if (tournamentSize <= 0) {
        cerr << "setTournamentSize: tournamentSize must be >= 1" << endl;
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
    mTournament.resize(mTournamentSize);
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
            if(mTournament[j].index == index) {
                match = true;
                break;
            }
        }
        if(!match) {
            mTournament[i].index = index;
            mTournament[i].fitnessValue = fitnessValues[index];
            i++;
        }
    }

    size_t kWinner = 0;
    if(mSelectionProbability < 1.0) {
        // choose the winner with a probability p, 2nd with p*(1-p), 3rd with p*(1-p)*2,...
        for(int j=0; j<mTournamentSize; j++) {
            float chooseP = mSelectionProbability * pow(1.0 - mSelectionProbability, j);
            if(Stat::randUniform() < chooseP) {
                kWinner = j;
            }
        }

        std::nth_element(mTournament.begin(), mTournament.begin()+kWinner, mTournament.end());
    } else {
        float minFitnessValue = mTournament[0].fitnessValue;
        for(int j=0; j<mTournamentSize; j++) {
            float value = mTournament[j].fitnessValue;
            if(value < minFitnessValue) {
                minFitnessValue = value;
                kWinner = j;
            }
        }

    }

    return mTournament[kWinner].index;
}
