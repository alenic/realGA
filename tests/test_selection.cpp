#include "testcommon.h"

SelectionTests::SelectionTests() {}

void SelectionTests::test_RWsearchIndexBinarySearch() {
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


void SelectionTests::test_RWselect() {
    int popSize = 100;
    int n_test = 50000;
    vector<float> fitnessValues(popSize);
    vector<float> indexA(n_test), indexB(n_test);

    for(int i=0; i<fitnessValues.size(); i++) {
        fitnessValues[i] = sin((float)i / 6.0);
    }
    RouletteWheelSelection selector = RouletteWheelSelection(fitnessValues.size());

    // allocate for performances
    selector.init(fitnessValues);

    for(int i=0; i<n_test; i++) {
        int A, B;
        selector.select(fitnessValues, A, B);
        indexA[i] = (float)A;
        indexB[i] = (float)B;
    }

    int meanA = 0, meanB =0;
    for(int i=0; i<n_test; i++) {
        meanA += indexA[i];
        meanB += indexB[i];
    }
    meanA /= n_test;
    meanB /= n_test;
    cout << "Mean A: " << meanA << "   Mean B:" << meanB << endl;

    printDistribution(indexA);
}