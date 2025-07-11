#include "algorithms.h"

namespace RALG
{

    void realgaSwap(float &a, float &b)
    {
        float c = a;
        a = b;
        b = c;
    }

    int partition(std::vector<float> &arr, int l, int r)
    {
        float x = arr[r];
        int i = l;
        for (int j = l; j <= r - 1; j++)
        {
            if (arr[j] <= x)
            {
                realgaSwap(arr[i], arr[j]);
                i++;
            }
        }
        realgaSwap(arr[i], arr[r]);
        return i;
    }

    float kthSmallest_(std::vector<float> &arr, int l, int r, int k)
    {
        if (k > 0 && k <= r - l + 1)
        {
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
            return kthSmallest_(arr, index + 1, r, k - index + l - 1);
        }

        // If k is more than number of
        // elements in array
        return -1;
    }

    float kthSmallest(vector<float> &fitnessValues, int l, int r, int k)
    {
        // Copy fitnessValues
        int n = fitnessValues.size();
        vector<float> arr(n);

        for (int i = 0; i < n; i++)
        {
            arr[i] = fitnessValues[i];
        }

        float kth = kthSmallest_(arr, l, r, k);

        return kth;
    }

    int argKthSmallest(vector<float> &fitnessValues, int l, int r, int k)
    {
        // additive O(N) to find the index
        int n = fitnessValues.size();
        float kth = kthSmallest(fitnessValues, l, r, k);

        int kthIndex = -1;

        for (int i = 0; i < n; i++)
        {
            if (kth == fitnessValues[i])
            {
                kthIndex = i;
                break;
            }
        }

        return kthIndex;
    }

    // An iterative binary search function.
    int searchIndexBinarySearch(vector<float> arr, float x, int left, int right)
    {
        int l = left, r = right;
        int m = 0;
        while (l < r)
        {
            m = (r + l) / 2;
            if (l == r)
            {
                return m;
            }
            // modified binary search to find the right index
            if (r == l + 1)
            {
                if (x > arr[l] && x < arr[r])
                    return l;
                if (x > arr[r])
                    return r;
            }
            // Check if x is present at mid
            if (arr[m] == x)
                return m;
            // If x greater, ignore left half
            if (arr[m] < x)
                l = m;
            // If x is smaller, ignore right half
            else
                r = m;
        }
        // If we reach here, then element was not present
        return m;
    }

    void minmax(vector<float> arr, float &minValue, float &maxValue)
    {
        minValue = arr[0];
        maxValue = arr[0];
        // compute minimum and maximum
        for (int i = 1; i < arr.size(); i++)
        {
            float v = arr[i];
            if (v < minValue)
                minValue = v;
            if (v > maxValue)
                maxValue = v;
        }
    }

    int argMin(float arr[], int l, int r)
    {
        float minValue = arr[0];
        int minIndex = 0;
        for (int i = l + 1; i <= r; i++)
        {
            if (arr[i] < minValue)
            {
                minValue = arr[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    int argSelection(float arr[], int l, int r, int kth)
    {
        // k * O(n)
        float minValue;
        int minIndex;
        // find min
        for (int k = 0; k < kth; k++)
        {
            minValue = arr[k];
            minIndex = k;
            for (int i = l + k + 1; i <= r; i++)
            {
                if (arr[i] < minValue)
                {
                    minValue = arr[i];
                    minIndex = i;
                }
            }
            if (k < kth - 1)
                realgaSwap(arr[k], arr[minIndex]);
        }
        return minIndex;
    }

} // RALG