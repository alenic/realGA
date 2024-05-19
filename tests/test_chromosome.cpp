#include "testcommon.h"

RealChromosomeTest::RealChromosomeTest() {

}

void RealChromosomeTest::test_randUniform() {
    cout << "RealChromosomeTest::test_randUniform" << endl;
    // bound test
    cout << "|---> bound test: ";
    RealChromosome g;
    vector<float> LB(40);
    vector<float> UB(40);
    for (int i = 0; i < 40; ++i) {
        LB[i] = 100 * rand() - 50;
        UB[i] = LB[i] + 10;
    }

    bool fail = false;
    for(int n=1; n<=40 && !fail; n++) {
        g.gene.resize(n);
        g.randUniform(LB, UB);
        for(int i=0; i<n; i++) {
            if(g.gene[i] < LB [i] || g.gene[i] > UB[i]) {
                fail = true;
                break;
            }
        }
    }
    if(fail)
        coutColor("FAILED", TEXT_RED);
    else
        coutColor("PASSED", TEXT_GREEN);
    cout << endl;
}


void RealChromosomeTest::test_bound() {

}

void RealChromosomeTest::test_distanceTo() {

}
