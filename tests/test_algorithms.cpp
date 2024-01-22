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
    vector<float> x = {5.0, 3.0, 1.0, 6.0, 2.0, 4.0};

    int index = RALG::argKthSmallest(x, 0, x.size()-1, 5);
    //cout << "Index " << index << endl;
    index = RALG::argKthSmallest(x, 0, x.size()-1, 1);
    //cout << "Index " << index << endl;
}