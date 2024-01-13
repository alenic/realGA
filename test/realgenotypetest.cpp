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
    g.setBounds(LB, UB);
    bool fail = false;
    for(int n=1; n<=40 && !fail; n++) {
        g.gene.resize(n);
        g.randUniform();
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
    // Destroy all elements
    g.gene.clear();
}


void RealChromosomeTest::test_uniformMutate() {
    cout << "RealChromosomeTest::test_uniformMutate" << endl;
    // Test the interval bound
    cout << "|---> 0.5 +- 0.2*rand() - : ";
    RealChromosome g(1);
    vector<float> LB = { 0.0 };
    vector<float> UB = { 1.0 };

    g.setBounds(LB, UB);
    bool fail = false;
    float minValue = 1.0;
    float maxValue = 0.0;
    for(int i=0; i<10000; i++) {
        g.gene[0] = 0.5;
        g.uniformMutate(0, 0.4);
        if(g.gene[0] < minValue) {
            minValue = g.gene[0];
        }
        if(g.gene[0] > maxValue) {
            maxValue = g.gene[0];
        }
        if(g.gene[0] < 0.3 || g.gene[0] > 0.7) {
            fail = true;
            break;
        }
    }
    cout << "min/max: [" << minValue << "," << maxValue << "] :";
    if(fail)
        coutColor("FAILED", TEXT_RED);
    else
        coutColor("PASSED", TEXT_GREEN);
    cout << endl;
}

void RealChromosomeTest::test_gaussianMutate() {

}

void RealChromosomeTest::test_bound() {

}

void RealChromosomeTest::test_distanceTo() {

}
