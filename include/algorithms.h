#ifndef REALGEN_ALGORITHMS_H
#define REALGEN_ALGORITHMS_H
#include <vector>

using namespace std;


namespace RALG {

void realgen_swap(float &a, float &b);
int partition(float arr[], int l, int r);
float kthSmallest_(float arr[], int l, int r, int k);
float kthSmallest(vector<float> &fitnessValues, int l, int r, int k);
int argKthSmallest(vector<float> &fitnessValues, int l, int r, int k);
int searchIndexBinarySearch(vector<float> arr, float x, int left, int right);
void minmax(vector<float> arr, float &minValue, float &maxValue);
}

#endif // REALGEN_ALGORITHMS_H
