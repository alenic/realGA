#include "testcommon.h"

void RALGTest::test_searchIndexBinarySearch()
{
    vector<float> array = {0, 1.3, 4.5, 5.2, 6.4, 7.9, 9.0, 11.0};
    bool passed = true;
    for (int i = 0; i < 100; i++)
    {
        float x = Stat::randUniform() * 15 - 1;
        // naive search
        int expected = 0;
        for (size_t j = 0; j < array.size(); j++)
        {
            if (x >= array[j])
            {
                expected = j;
            }
            else
            {
                break;
            }
        }
        int index = RALG::searchIndexBinarySearch(array, x, 0, array.size() - 1);

        if (expected != index)
        {
            coutColor("test_searchIndexBinarySearch", "FAILED", TEXT_RED);
            cout << x << " founded index " << index << " , expected index " << expected << endl;
            passed = false;
            break;
        }
    }
    cout << "test_RWsearchIndexBinarySearch ";
    if (passed)
    {
        coutColor("test_searchIndexBinarySearch", "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor("test_searchIndexBinarySearch", "FAILED", TEXT_RED);
    }
}

void RALGTest::test_argKthSmallest()
{
    vector<float> x(100);
    bool passed = true;
    int index;

    for (size_t i = 0; i < x.size(); i++)
    {
        x[i] = (float)(i + 1);
    }

    for (size_t i = 0; i < x.size(); i++)
    {
        index = RALG::argKthSmallest(x, 0, x.size() - 1, i + 1);
        if (index != (int)i)
        {
            passed = false;
            break;
        }
    }
    if (passed)
    {
        coutColor("test_argKthSmallest", "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor("test_argKthSmallest", "FAILED", TEXT_RED);
    }
}

void RALGTest::test_kthSmallest()
{
    vector<float> x = {9.0f, 2.0f, 5.0f, 1.0f, 7.0f, 3.0f, 6.0f, 4.0f, 8.0f};
    vector<float> sorted = x;
    std::sort(sorted.begin(), sorted.end());

    bool passed = true;
    for (size_t k = 1; k <= sorted.size(); ++k)
    {
        vector<float> copy = x;
        float val = RALG::kthSmallest(copy, 0, copy.size() - 1, k);
        if (val != sorted[k - 1])
        {
            passed = false;
            cout << "Expected " << sorted[k - 1] << ", got " << val << endl;
            break;
        }
    }

    passed ? coutColor("test_kthSmallest", "PASSED", TEXT_GREEN) : coutColor("test_kthSmallest", "FAILED", TEXT_RED);
}

void RALGTest::test_argMin()
{
    float arr[] = {10.0f, 9.0f, 3.5f, 5.2f, -1.0f, 4.0f};
    int index = RALG::argMin(arr, 0, 5);
    bool passed = (index == 4);

    passed ? coutColor("test_argMin", "PASSED", TEXT_GREEN) : coutColor("test_argMin", "FAILED", TEXT_RED);
}

void RALGTest::test_minmax()
{
    vector<float> x = {1.0f, -5.0f, 3.0f, 9.0f, 0.0f, 7.0f};
    float minVal, maxVal;
    RALG::minmax(x, minVal, maxVal);

    bool passed = (minVal == -5.0f && maxVal == 9.0f);

    passed ? coutColor("test_minmax", "PASSED", TEXT_GREEN) : coutColor("test_minmax", "FAILED", TEXT_RED);
}
