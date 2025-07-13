#include "algorithms.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <numeric>

namespace RALG
{
    namespace
    {
        constexpr float FLOAT_EPSILON = std::numeric_limits<float>::epsilon();
        constexpr int INVALID_INDEX = -1;
    }

    int partition(std::vector<float> &arr, int l, int r)
    {
        if (l >= r || arr.empty())
        {
            throw std::invalid_argument("Invalid partition bounds");
        }

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
        // Validate input parameters
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }
        if (l < 0 || r >= static_cast<int>(arr.size()) || l > r)
        {
            throw std::out_of_range("Invalid array bounds");
        }
        if (k <= 0 || k > r - l + 1)
        {
            throw std::out_of_range("k is out of valid range");
        }

        // Use nth_element for efficient kth smallest finding
        std::vector<float> copy(arr.begin() + l, arr.begin() + r + 1);
        std::nth_element(copy.begin(), copy.begin() + k - 1, copy.end());
        return copy[k - 1];
    }

    int argKthSmallest(const std::vector<float> &arr, int l, int r, int k)
    {
        // Validate input parameters
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }
        if (l < 0 || r >= static_cast<int>(arr.size()) || l > r)
        {
            throw std::out_of_range("Invalid array bounds");
        }
        if (k <= 0 || k > r - l + 1)
        {
            throw std::out_of_range("k is out of valid range");
        }

        // Create vector of indices paired with values
        std::vector<std::pair<float, int>> indexed_values;
        indexed_values.reserve(r - l + 1);

        for (int i = l; i <= r; ++i)
        {
            indexed_values.emplace_back(arr[i], i);
        }

        // Find kth smallest using nth_element
        std::nth_element(indexed_values.begin(),
                         indexed_values.begin() + k - 1,
                         indexed_values.end());

        return indexed_values[k - 1].second;
    }

    int searchIndexBinarySearch(const std::vector<float> &arr, float x, int left, int right)
    {
        // Validate input
        if (arr.empty())
        {
            return INVALID_INDEX;
        }
        if (left < 0 || right >= static_cast<int>(arr.size()) || left > right)
        {
            return INVALID_INDEX;
        }

// Verify array is sorted (in debug mode)
#ifdef DEBUG
        if (!std::is_sorted(arr.begin() + left, arr.begin() + right + 1))
        {
            throw std::logic_error("Array must be sorted for binary search");
        }
#endif

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            // Use epsilon for floating point comparison
            if (std::abs(arr[mid] - x) < FLOAT_EPSILON)
            {
                return mid;
            }
            else if (arr[mid] < x)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }

        return INVALID_INDEX;
    }

    void minmax(const std::vector<float> &arr, float &minValue, float &maxValue)
    {
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }

        auto result = std::minmax_element(arr.begin(), arr.end());
        minValue = *result.first;
        maxValue = *result.second;
    }

    int argMin(const float arr[], int l, int r)
    {
        if (arr == nullptr)
        {
            throw std::invalid_argument("Array pointer cannot be null");
        }
        if (l > r)
        {
            throw std::invalid_argument("Invalid array bounds");
        }

        const float *minPtr = std::min_element(arr + l, arr + r + 1);
        return static_cast<int>(minPtr - arr);
    }

    // Additional utility functions for better algorithm support
    int argMax(const std::vector<float> &arr)
    {
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }

        auto maxIt = std::max_element(arr.begin(), arr.end());
        return static_cast<int>(std::distance(arr.begin(), maxIt));
    }

    int argMin(const std::vector<float> &arr)
    {
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }

        auto minIt = std::min_element(arr.begin(), arr.end());
        return static_cast<int>(std::distance(arr.begin(), minIt));
    }

    // Efficient partial sort for elitism
    void partialSort(std::vector<float> &arr, int k)
    {
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }
        if (k <= 0 || k > static_cast<int>(arr.size()))
        {
            throw std::out_of_range("k is out of valid range");
        }

        std::partial_sort(arr.begin(), arr.begin() + k, arr.end());
    }

    // Get indices of k smallest elements efficiently
    std::vector<int> argKSmallest(const std::vector<float> &arr, int k)
    {
        if (arr.empty())
        {
            throw std::invalid_argument("Array cannot be empty");
        }
        if (k <= 0 || k > static_cast<int>(arr.size()))
        {
            throw std::out_of_range("k is out of valid range");
        }

        // Create vector of indices
        std::vector<int> indices(arr.size());
        std::iota(indices.begin(), indices.end(), 0);

        // Partial sort indices based on array values
        std::partial_sort(indices.begin(), indices.begin() + k, indices.end(),
                          [&arr](int i, int j)
                          { return arr[i] < arr[j]; });

        indices.resize(k);
        return indices;
    }

} // namespace RALG