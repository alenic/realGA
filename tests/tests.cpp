#include "testcommon.h"
#define MAX_RESULTS_BUFFER 16

int main()
{
    RealGAOptions opt;

    StatTest statTest;
    RALGTest ralgTest;
    RealChromosomeTest realChromosomeUT;
    SelectionTests selectionUT;

    cout << "==================== Statistical Test ============================" << endl;
    // test uniform distribution
    statTest.test_randUniform(0.0f, 1.0f);
    statTest.test_randUniform(-1.0f, 1.0f);
    // test gaussian distribution
    for (auto mean : {-1.0f, 0.0f, 1.0f})
    {
        for (auto sigma : {0.01f, 0.1f, 1.0f, 10.0f})
        {
            statTest.test_randGaussian(mean, sigma);
        }
    }

    // statTest.test_randIndex();
    // statTest.test_randInteger();
    statTest.test_smallSigmaGaussian();
    /*
    cout << "==================== Algorithms Test ============================" << endl;
    ralgTest.test_argKthSmallest();
    ralgTest.test_searchIndexBinarySearch();
    ralgTest.test_argMin();
    ralgTest.test_kthSmallest();
    ralgTest.test_minmax();

    cout << "==================== RealChromosome Test ============================" << endl;
    realChromosomeUT.test_randUniform();

    cout << "==================== Selection Test ============================" << endl;

    selectionUT.test_roulette_select();
    selectionUT.test_tournament_select();
    */
    return 0;
}
