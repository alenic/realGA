#include "algorithms.h"
#include <algorithm> // for std::swap
#include <vector>
#include <cmath> // for std::abs if needed
#include <cassert>

namespace RALG
{

    int partition(std::vector<float> &arr, int l, int r)
    {
        float x = arr[r];
        int i = l;
        for (int j = l; j <= r - 1; j++)
        {
            if (arr[j] <= x)
            {
                std::swap(arr[i], arr[j]);
                i++;
            }
        }
        std::swap(arr[i], arr[r]);
        return i;
    }

    float kthSmallest(const std::vector<float> &arr, int l, int r, int k)
    {
        if (k <= 0 || k > r - l + 1)
            return -1.0f;

        std::vector<float> copy(arr.begin() + l, arr.begin() + r + 1);
        std::nth_element(copy.begin(), copy.begin() + k - 1, copy.end());
        return copy[k - 1];
    }

    int argKthSmallest(const std::vector<float> &arr, int l, int r, int k)
    {
        float kth = kthSmallest(arr, l, r, k);
        for (int i = l; i <= r; ++i)
        {
            if (arr[i] == kth)
                return i;
        }
        return -1;
    }

    int searchIndexBinarySearch(const std::vector<float> &arr, float x, int left, int right)
    {
        if (arr.empty() || left > right)
            return -1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (arr[mid] == x)
                return mid;
            else if (arr[mid] < x)
                left = mid + 1;
            else
                right = mid - 1;
        }

        // Not found, return -1 (standard behavior)
        return -1;
    }

    void minmax(const std::vector<float> &arr, float &minValue, float &maxValue)
    {
        assert(!arr.empty());
        auto result = std::minmax_element(arr.begin(), arr.end());
        minValue = *result.first;
        maxValue = *result.second;
    }

    int argMin(const float arr[], int l, int r)
    {
        // Use std::min_element for efficiency and clarity
        const float *minPtr = std::min_element(arr + l, arr + r + 1);
        return static_cast<int>(minPtr - arr);
    }

} // namespace RALG
