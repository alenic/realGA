#include "selection.h"


TournamentSelection::TournamentSelection(int populationSize) {
    mTorunamentSize = (int)(populationSize*0.1);
    mPopulationSize = populationSize;
    if (mTorunamentSize == 0) mTorunamentSize = 1;
    mSelectionProbability = 0.85;
}

TournamentSelection::~TournamentSelection() {

}

void TournamentSelection::setTournamentSize(int tournamentSize) {
    if (tournamentSize <= 0) {
        cerr << "setTournamentSize: tournamentSize must be >= 1" << endl;
        exit(-1);
    }
    mTorunamentSize = tournamentSize;
}

void TournamentSelection::setSelectionProbability(float selectionProbability) {
    if((selectionProbability <= 0.0) || (selectionProbability>1.0)) {
        cerr << "TournamentSelection: selectionProbability must be in (0,1]" << endl;
        exit(-1);
    }
    mSelectionProbability = selectionProbability;
}

void TournamentSelection::init(vector<float> &fitnessValues) {
    mTournament.resize(mTorunamentSize);
}

void TournamentSelection::select(vector<float> &fitnessValues, int &indexA, int &indexB) {
    // choose randomly mTournamentSize fitness value and get the 1st and 2nd winners
    indexA = tournament(fitnessValues);
    indexB = tournament(fitnessValues);
    if(indexA == indexB) {
        if(indexA != fitnessValues.size()-1) {
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
    // sample random mTorunamentSize indices from mPopulationSize indices, without replacement
    while(i < mTorunamentSize) {
        int index = Stat::randIndex(mPopulationSize);
        
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

    // choose the winner with a probability p, 2nd with p*(1-p), 3rd with p*(1-p)*2,... 
    sort(mTournament.begin(), mTournament.end());
    
    for(int j=0; j<mTorunamentSize; j++) {
        float chooseP = mSelectionProbability * pow(1.0 - mSelectionProbability, j);
        if(Stat::randUniform() < chooseP) {
            return mTournament[j].index;
        }
    }

}
