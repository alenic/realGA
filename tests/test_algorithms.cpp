#include "testcommon.h"

RALGTest::RALGTest() {

}

void RALGTest::test_argKthSmallest() {
    vector<float> x = {5.0, 3.0, 1.0, 6.0, 2.0};

    int index = RALG::argKthSmallest(x, 0, x.size()-1, 5);
    cout << "Index " << index << endl;
    index = RALG::argKthSmallest(x, 0, x.size()-1, 1);
    cout << "Index " << index << endl;
}