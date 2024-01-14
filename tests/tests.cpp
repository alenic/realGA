#include "testcommon.h"
#define MAX_RESULTS_BUFFER 16

int main() {
    RealGenOptions opt;

    StatTest statUT;
    RealChromosomeTest realChromosomeUT;
    SelectionTests selectionUT;

    cout << "==================== Statistical Test ============================" << endl;
    statUT.test_uniformDist();
    statUT.test_gaussianDist(5.0, 2.0);

    cout << "==================== RealChromosome Test ============================" << endl;
    realChromosomeUT.test_randUniform();
    realChromosomeUT.test_uniformMutate();

    cout << "==================== Selection Test ============================" << endl;
    selectionUT.test_RWsearchIndexBinarySearch();
    selectionUT.test_RWselect();
    

    return 0;
}
