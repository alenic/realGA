#include "testcommon.h"

RealGenotypeTest::RealGenotypeTest() {

}

void 	RealGenotypeTest::test_uniformRandom() {
	cout << "RealGenotypeTest::test_uniformRandom" << endl;
	// bound test
	cout << "|---> [0,1] - bound test: ";
	RealGenotype g;
	bool fail = false;
	for(int n=1; n<=40 && !fail; n++) {
		g.gene.resize(n);
		g.uniformRandom();
		for(int i=0; i<n; i++) {
			if(g.gene[i] < 0.0 || g.gene[i] > 1.0) {
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


void 	RealGenotypeTest::test_uniformLocalRandom() {
	cout << "RealGenotypeTest::test_uniformLocalRandom" << endl;
	// Test the interval bound
	cout << "|---> 0.5 +- 0.2*rand() - : ";
	RealGenotype g(1);
	bool fail = false;
	float minValue = 1.0;
	float maxValue = 0.0;
	for(int i=0; i<10000; i++) {
		g.gene[0] = 0.5;
		g.uniformLocalRandom(0, 0.4);
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

void 	RealGenotypeTest::test_gaussianLocalRandom() {

}

void 	RealGenotypeTest::test_bound() {

}

void 	RealGenotypeTest::test_distanceTo() {

}