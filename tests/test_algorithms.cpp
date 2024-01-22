#include "testcommon.h"

RALGTest::RALGTest() {

}


void RALGTest::test_searchIndexBinarySearch() {
    vector<float> array = {0, 1.3, 4.5, 5.2, 6.4, 7.9, 9.0, 11.0};
    bool passed=true;
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
        int index = RALG::searchIndexBinarySearch(array, x, 0, array.size()-1);
        
        if(expected != index) {
            coutColor("FAILED", TEXT_RED);
            cout << x << " founded index " << index << " , expected index " << expected << endl;
            passed = false;
            break;
        }
    }
    cout << "test_RWsearchIndexBinarySearch ";
    if(passed){
        coutColor("PASSED", TEXT_GREEN);
    } else {
        coutColor("FAILED", TEXT_RED);
    }
}

void RALGTest::test_argKthSmallest() {
    vector<float> x(100);
    bool passed = true;
    int index;

    for(int i=0; i<x.size(); i++) {
        x[i] = (float)(i+1);
    }
    
    for(int i=0; i<x.size(); i++) {
        index = RALG::argKthSmallest(x, 0, x.size()-1, i+1);
        if (index != i) {
            passed = false;
            break;
        }
    }
    if(passed){
        coutColor("PASSED", TEXT_GREEN);
    } else {
        coutColor("FAILED", TEXT_RED);
    }
}