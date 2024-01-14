#include "selection.h"


TournamentSelection::TournamentSelection() {
    mTorunamentSize = 32;
}

TournamentSelection::TournamentSelection(int tournamentSize) {
    mTorunamentSize = tournamentSize;
}

TournamentSelection::~TournamentSelection() {

}

void TournamentSelection::init(vector<float> &fitnessValues) {
    mPopulationSize = fitnessValues.size();
}

void TournamentSelection::select(vector<float> &fitnessValues, int &indexA, int &indexB) {
    // choose randomly mTournamentSize fitness value and get the 1st and 2nd winners
    float minIndexA=0, minValueA=fitnessValues[0];
    float minIndexB=0;
    int index = 0;
    for(int i=0; i<mTorunamentSize; i++) {
        index = Stat::randIndex(mPopulationSize-1);
        float value = fitnessValues[index];
        if (minIndexA < value) {
            minIndexB = minIndexA;
            minValueA = value;
            minIndexA = index;
        }
    }
    indexA = minIndexA;
    indexB = minIndexB;
    if(indexA == indexB) {
        if(indexA != mPopulationSize-1) {
            indexB = indexA + 1;
        } else {
            indexB = indexA - 1;
        }
    }
}
