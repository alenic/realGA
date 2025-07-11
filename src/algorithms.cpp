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

    float kthSmallest(std::vector<float> &arr, int l, int r, int k)
    {
        if (k > 0 && k <= r - l + 1)
        {
            int index = partition(arr, l, r);
            if (index - l == k - 1)
                return arr[index];
            if (index - l > k - 1)
                return kthSmallest(arr, l, index - 1, k);
            return kthSmallest(arr, index + 1, r, k - index + l - 1);
        }
        return -1.0f;
    }

    int argKthSmallest(const std::vector<float> &arr, int l, int r, int k)
    {
        std::vector<float> copy(arr.begin(), arr.end());
        float kth = kthSmallest(copy, l, r, k);
        for (int i = 0; i < (int)arr.size(); i++)
        {
            if (arr[i] == kth)
            {
                return i;
            }
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

        return std::max(0, left - 1);
    }

    void minmax(const std::vector<float> &arr, float &minValue, float &maxValue)
    {
        assert(!arr.empty());
        minValue = arr[0];
        maxValue = arr[0];
        for (size_t i = 1; i < arr.size(); i++)
        {
            float v = arr[i];
            if (v < minValue)
                minValue = v;
            if (v > maxValue)
                maxValue = v;
        }
    }

    int argMin(const float arr[], int l, int r)
    {
        float minValue = arr[l];
        int minIndex = l;
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

} // namespace RALG
