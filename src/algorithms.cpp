#include "algorithms.h"


namespace RALG {

void realgen_swap(float &a, float &b) {
    float c = a;
    a = b;
    b = c;
}

int partition(float arr[], int l, int r) 
{ 
    float x = arr[r];
    int i = l; 
    for (int j = l; j <= r - 1; j++) { 
        if (arr[j] <= x) { 
            realgen_swap(arr[i], arr[j]); 
            i++; 
        } 
    } 
    realgen_swap(arr[i], arr[r]); 
    return i; 
}

float kthSmallest_(float arr[], int l, int r, int k) 
{ 
    if (k > 0 && k <= r - l + 1) { 
        // Partition the array around last  
        // element and get position of pivot  
        // element in sorted array 
        int index = partition(arr, l, r); 
  
        // If position is same as k 
        if (index - l == k - 1) 
            return arr[index]; 
  
        // If position is more, recur  
        // for left subarray 
        if (index - l > k - 1)  
            return kthSmallest_(arr, l, index - 1, k); 
  
        // Else recur for right subarray 
        return kthSmallest_(arr, index + 1, r,  k - index + l - 1); 
    }
  
    // If k is more than number of  
    // elements in array 
    return -1; 
}


float kthSmallest(vector<float> &fitnessValues, int l, int r, int k) {
    // Copy fitnessValues
    int n = fitnessValues.size();
    float *arr = new float[n];

    for(int i=0; i<n; i++) {
        arr[i] = fitnessValues[i];
    }

    float kth = kthSmallest_(arr, l, r, k);

    delete []arr;

    return kth;
}

int argKthSmallest(vector<float> &fitnessValues, int l, int r, int k) {
    // additive O(N) to find the index
    int n = fitnessValues.size();
    float kth = kthSmallest(fitnessValues, l, r, k);

    int kthIndex = 0;

    for(int i=0; i<n; i++) {
        if(kth == fitnessValues[i]) {
            kthIndex = i;
            break;
        }
    }

    return kthIndex;
}




}