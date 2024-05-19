#include "testcommon.h"
#include <math.h>

SelectionTests::SelectionTests() {}

void SelectionTests::test_roulette_select() {
    int popSize = 100;
    int numSelect = 100000;
    vector<float> fitnessValues(popSize);
    vector<float> indexA(numSelect),
                  indexB(numSelect),
                  fitnessValuesSelectedA(numSelect),
                  fitnessValuesSelectedB(numSelect);

    for(int i=0; i<popSize; i++) {
        float time = (float)i / ((float)popSize-1);
        fitnessValues[i] = sin(time * M_PI);
    }

    std::random_shuffle(fitnessValues.begin(), fitnessValues.end());

    RouletteWheelSelection selector = RouletteWheelSelection(popSize);

    for(int i=0; i<numSelect; i++) {
        int A, B;
        selector.select(fitnessValues, A, B);
        fitnessValuesSelectedA[i] = fitnessValues[A];
        fitnessValuesSelectedB[i] = fitnessValues[B];
        indexA[i] = (float)A;
        indexB[i] = (float)B;
    }

    printDistribution(fitnessValuesSelectedA, 10, 100);
    printDistribution(fitnessValuesSelectedB, 10, 100);
}



void SelectionTests::test_tournament_select() {
    int popSize = 100;
    int numSelect = 10000;
    vector<float> fitnessValues(popSize);
    vector<float> indexA(numSelect),
                  indexB(numSelect),
                  fitnessValuesSelectedA(numSelect),
                  fitnessValuesSelectedB(numSelect);

    for(int i=0; i<popSize; i++) {
        float time = (float)i / ((float)popSize-1);
        fitnessValues[i] = sin(time * M_PI);
    }

    TournamentSelection selector = TournamentSelection(popSize);

    for(int i=0; i<numSelect; i++) {
        int A, B;
        selector.select(fitnessValues, A, B);
        fitnessValuesSelectedA[i] = fitnessValues[A];
        fitnessValuesSelectedB[i] = fitnessValues[B];
        indexA[i] = (float)A;
        indexB[i] = (float)B;
    }
    printDistribution(fitnessValuesSelectedA, 10, 100);
    printDistribution(fitnessValuesSelectedB, 10, 100);
}