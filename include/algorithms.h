/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        realGA algorithms for general purpose
---------------------------------------------
*/
#ifndef REALGA_ALGORITHMS_H
#define REALGA_ALGORITHMS_H
#include <vector>

using namespace std;


namespace RALG {

void realgaSwap(float &a, float &b);
int partition(float arr[], int l, int r);
float kthSmallest_(float arr[], int l, int r, int k);
float kthSmallest(vector<float> &fitnessValues, int l, int r, int k);
int argKthSmallest(vector<float> &fitnessValues, int l, int r, int k);
int searchIndexBinarySearch(vector<float> arr, float x, int left, int right);
void minmax(vector<float> arr, float &minValue, float &maxValue);
int argMin(float arr[], int l, int r);
int argSelection(float arr[], int l, int r, int kth);
}

#endif // REALGA_ALGORITHMS_H
