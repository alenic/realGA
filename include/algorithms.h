#ifndef REALGEN_ALGORITHMS_H
#define REALGEN_ALGORITHMS_H
#include <vector>

using namespace std;


namespace RALG {

void realgen_swap(float &a, float &b);
int partition(float arr[], int l, int r);
float kthSmallest_(float arr[], int l, int r, int k);
int kthSmallest(vector<float> &fitnessValues, int l, int r, int k);
int argKthSmallest(vector<float> &fitnessValues, int l, int r, int k);
}

#endif // REALGEN_ALGORITHMS_H
