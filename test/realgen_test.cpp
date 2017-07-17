#include "testcommon.h"

RealGenotypeTest::RealGenotypeTest() {

}

void 	RealGenotypeTest::test_uniformRandom() {
	cout << "RealGenotypeTest::test_uniformRandom: ";
	bool fail = false;
	for(int n=1; n<=10; n++) {
		g.gene.resize(n);
		g.uniformRandom();
		for(int i=0; i<n; i++) {
			if(g.gene[i] < 0.0 || g.gene[i] > 1.0) {
				fail = true;
				break;
			}
			if(fail) break;
		}
	}
	if(fail)
		coutColor("FAILED", TEXT_RED);
	else
		coutColor("PASSED", TEXT_GREEN);
	cout << endl;
}


void 	RealGenotypeTest::test_uniformLocalRandom() {

}

void 	RealGenotypeTest::test_gaussianLocalRandom() {

}

void 	RealGenotypeTest::test_bound() {

}

void 	RealGenotypeTest::test_distanceTo() {

}