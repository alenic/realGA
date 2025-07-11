/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        RealGA algorithms for general purpose
---------------------------------------------
*/
#ifndef REALGA_ALGORITHMS_H
#define REALGA_ALGORITHMS_H
#include <vector>

using namespace std;

namespace RALG
{

    int partition(std::vector<float> &arr, int l, int r);
    float kthSmallest(vector<float> &arr, int l, int r, int k);
    int argKthSmallest(const vector<float> &arr, int l, int r, int k);
    int searchIndexBinarySearch(const vector<float> &arr, float x, int left, int right);
    void minmax(const vector<float> &arr, float &minValue, float &maxValue);
    int argMin(const float arr[], int l, int r);
}

#endif // REALGA_ALGORITHMS_H
