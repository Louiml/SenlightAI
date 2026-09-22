// Write a C++ function named `slidingWindowMaxima` that takes a vector of integers and a positive integer `k` (the window size) as input, and returns a vector of integers containing the maximum value in every contiguous subarray (window) of length `k`. If `k` is greater than the array size or `k <= 0`, the function should return an empty vector. The function must handle arrays with negative numbers, duplicate values, and any array length from 1 upward. The solution should process each window independently without reusing results from previous windows, matching the brute‑force behavior of the given snippet, but must be implemented as a clean, reusable free function with proper `const` correctness.

// The core algorithm is straightforward: for each possible starting index `i` from `0` to `n - k` (inclusive), scan the next `k` elements and keep track of the largest value found. Append that maximum to the result vector. This is a classic brute‑force sliding‑window maximum, which is easy to verify and understand. Edge cases include: (1) when `k` is larger than the array length, there are no valid windows, so return an empty vector; (2) when `k <= 0`, the problem is ill‑defined, so return an empty vector; (3) arrays with all negative values still produce correct maxima because we initialize the current maximum to the first element of the window rather than to a sentinel like `INT_MIN`; (4) duplicate values work naturally because `max` will simply return the same value repeatedly. The time complexity is \(O(n \cdot k)\) because for each of the \(n-k+1\) windows we perform \(k\) comparisons. The space complexity is \(O(n-k+1)\) for the output vector, plus \(O(1)\) auxiliary space for temporary variables. This matches the behavior of the given snippet exactly, and the free‑function design makes it testable and reusable.

#include <vector>
#include <algorithm>

// Return a vector containing the maximum of every contiguous subarray of length k.
// If k is non-positive or larger than the input vector's size, return an empty vector.
std::vector<int> slidingWindowMaxima(const std::vector<int>& arr, int k) {
    std::vector<int> result;
    int n = static_cast<int>(arr.size());
    
    if (k <= 0 || k > n) {
        return result;
    }
    
    for (int i = 0; i <= n - k; ++i) {
        int current_max = arr[i];
        for (int j = i + 1; j < i + k; ++j) {
            current_max = std::max(current_max, arr[j]);
        }
        result.push_back(current_max);
    }
    
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// This main function tests the slidingWindowMaxima function.
int main() {
    // Basic case with positive numbers
    std::vector<int> result1 = slidingWindowMaxima({1, 3, -1, -3, 5, 3, 6, 7}, 3);
    assert(result1 == std::vector<int>({3, 3, 5, 5, 6, 7}));
    
    // Window size equals array length
    std::vector<int> result2 = slidingWindowMaxima({4, 2, 8}, 3);
    assert(result2 == std::vector<int>({8}));
    
    // Window size equals 1 (each element is its own maximum)
    std::vector<int> result3 = slidingWindowMaxima({-5, -1, -3}, 1);
    assert(result3 == std::vector<int>({-5, -1, -3}));
    
    // All negative numbers
    std::vector<int> result4 = slidingWindowMaxima({-10, -20, -15}, 2);
    assert(result4 == std::vector<int>({-10, -15}));
    
    // Duplicate values
    std::vector<int> result5 = slidingWindowMaxima({7, 7, 7, 7}, 2);
    assert(result5 == std::vector<int>({7, 7, 7}));
    
    // k greater than array size returns empty
    std::vector<int> result6 = slidingWindowMaxima({1, 2}, 3);
    assert(result6.empty());
    
    // k is zero or negative returns empty
    std::vector<int> result7 = slidingWindowMaxima({1, 2, 3}, 0);
    assert(result7.empty());
    
    // Single element array with k=1
    std::vector<int> result8 = slidingWindowMaxima({42}, 1);
    assert(result8 == std::vector<int>({42}));
    
    // Larger test with increasing then decreasing pattern
    std::vector<int> result9 = slidingWindowMaxima({1, 2, 3, 4, 5, 4, 3, 2, 1}, 4);
    assert(result9 == std::vector<int>({4, 5, 5, 5, 5, 4}));
    
    // Example from the original snippet: n=5, arr={5,3,1,4,2}, k=2 → {5,3,4,4}
    std::vector<int> result10 = slidingWindowMaxima({5, 3, 1, 4, 2}, 2);
    assert(result10 == std::vector<int>({5, 3, 4, 4}));
    
    return 0;
}
