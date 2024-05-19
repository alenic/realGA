#include "testcommon.h"
#define MAX_RESULTS_BUFFER 16

int main() {
    RealGAOptions opt;

    StatTest statTest;
    RALGTest ralgTest;
    RealChromosomeTest realChromosomeUT;
    SelectionTests selectionUT;

    cout << "==================== Statistical Test ============================" << endl;
    statTest.test_uniformDist();
    statTest.test_gaussianDist(5.0, 2.0);

    cout << "==================== Algorithms Test ============================" << endl;
    ralgTest.test_argKthSmallest();
    ralgTest.test_searchIndexBinarySearch();

    cout << "==================== RealChromosome Test ============================" << endl;
    realChromosomeUT.test_randUniform();

    cout << "==================== Selection Test ============================" << endl;
    
    selectionUT.test_roulette_select();
    selectionUT.test_tournament_select();

    return 0;
}
