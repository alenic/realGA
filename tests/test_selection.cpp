#include "testcommon.h"
#include <math.h>

SelectionTests::SelectionTests() {}

void SelectionTests::test_roulette_searchIndexBinarySearch() {
    vector<float> array = {0, 1.3, 4.5, 5.2, 6.4, 7.9, 9.0, 11.0};
    RouletteWheelSelection selector = RouletteWheelSelection(array.size());

    for(int i=0; i<100; i++) {
        float x = Stat::randUniform()*15 - 1;
        // naive search
        int expected = 0;
        for(int j=0; j<array.size(); j++) {
            if (x >= array[j]) {
                expected = j;
            } else {
                break;
            }
        }
        int index = selector.searchIndexBinarySearch(array, x);
        
        if(expected != index) {
            coutColor("FAILED", TEXT_RED);
            cout << x << " founded index " << index << " , expected index " << expected << endl;
            break;
        }
    }
    cout << "test_RWsearchIndexBinarySearch ";
    coutColor("PASSED", TEXT_GREEN);
}


void SelectionTests::test_roulette_select() {
    int popSize = 100;
    int numSelect = 100000;
    vector<float> fitnessValues(popSize);
    vector<float> indexA(numSelect), indexB(numSelect);

    for(int i=0; i<popSize; i++) {
        float time = (float)i / ((float)popSize-1);
        fitnessValues[i] = sin(time * M_PI);
    }
    RouletteWheelSelection selector = RouletteWheelSelection(popSize);

    // allocate for performances
    selector.init(fitnessValues);

    for(int i=0; i<numSelect; i++) {
        int A, B;
        selector.select(fitnessValues, A, B);
        indexA[i] = (float)A;
        indexB[i] = (float)B;
    }

    int meanA = 0, meanB =0;
    for(int i=0; i<numSelect; i++) {
        meanA += indexA[i];
        meanB += indexB[i];
    }
    meanA /= numSelect;
    meanB /= numSelect;
    cout << "Mean indexA: " << meanA << endl;
    printDistribution(indexA, 10, 100);
    cout << "Mean indexB: " << meanB << endl;
    printDistribution(indexB, 10, 100);
}



void SelectionTests::test_tournament_select() {
    int popSize = 100;
    int numSelect = 10000;
    vector<float> fitnessValues(popSize);
    vector<float> indexA(numSelect), indexB(numSelect);
    int tournamentSize = 32;

    for(int i=0; i<popSize; i++) {
        float time = (float)i / ((float)popSize-1);
        fitnessValues[i] = sin(time * M_PI);
    }

    TournamentSelection selector = TournamentSelection(tournamentSize);

    // allocate for performances
    selector.init(fitnessValues);

    for(int i=0; i<numSelect; i++) {
        int A, B;
        selector.select(fitnessValues, A, B);
        indexA[i] = (float)A;
        indexB[i] = (float)B;
    }

    int meanA = 0, meanB =0;
    for(int i=0; i<numSelect; i++) {
        meanA += indexA[i];
        meanB += indexB[i];
    }
    meanA /= numSelect;
    meanB /= numSelect;
    cout << "Mean indexA: " << meanA << endl;
    printDistribution(indexA, 10, 100);
    cout << "Mean indexB: " << meanB << endl;
    printDistribution(indexB, 10, 100);
}