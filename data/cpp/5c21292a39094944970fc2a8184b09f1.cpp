// Write a standalone C++ function that takes an integer array (as a `std::vector<int>`) and a positive integer `n` as the maximum allowed value (inclusive). The function should count the frequency of each element that appears in the array, but only for elements whose value lies in the range `[0, n]`. For any element outside this range, it should be completely ignored (not counted, and no error). The function should return a `std::vector<int>` of size `n+1` where index `i` holds the count of occurrences of `i` in the input array. The input array may be empty, may contain duplicates, may contain negative numbers, and may contain values much larger than `n`. Assume `n` is non-negative. Do not use any global arrays; allocate the result vector dynamically.

// The core idea is to leverage an auxiliary array (or vector) of size `n+1`, initialized to zeros, which serves as a frequency table. For each element in the input, check if it is within the valid range `[0, n]`. If it is, increment the corresponding counter. This approach works because the frequency table is indexed directly by the element value. Edge cases include an empty input array (return a vector of zeros), elements that are negative or exceed `n` (skip them), and `n = 0` (only element `0` is counted). Duplicates are naturally handled by incrementing the same counter multiple times. The algorithm runs in \(O(m)\) time where \(m\) is the number of elements in the input array, since we traverse it once and perform constant-time operations for each valid element. The auxiliary space is \(O(n)\) for the frequency vector, independent of the input size. No sorting or hash maps are needed, and the direct array indexing makes it very efficient for moderate `n`.

#include <vector>

// Count frequencies of elements in the range [0, n] within the input array.
// Elements outside [0, n] are ignored. Returns a vector of size n+1.
std::vector<int> countFrequencies(const std::vector<int>& arr, int n) {
    // Initialize frequency vector with n+1 zeros
    std::vector<int> freq(n + 1, 0);

    // Traverse the input array and count valid elements
    for (int value : arr) {
        // Only count if value is within [0, n]
        if (value >= 0 && value <= n) {
            ++freq[value];
        }
    }

    return freq;
}

#include <cassert>
#include <vector>
#include <iostream>

// Declaration (copied from solution for self-contained test)
std::vector<int> countFrequencies(const std::vector<int>& arr, int n);

int main() {
    // Basic case with duplicates
    std::vector<int> arr1 = {1, 2, 3, 2, 1, 1, 0};
    std::vector<int> result1 = countFrequencies(arr1, 3);
    assert(result1 == std::vector<int>({1, 3, 2, 1})); // index 0:1, 1:3, 2:2, 3:1

    // Empty array
    std::vector<int> arr2;
    std::vector<int> result2 = countFrequencies(arr2, 5);
    assert(result2 == std::vector<int>({0, 0, 0, 0, 0, 0}));

    // Elements outside range (negative and larger than n) are ignored
    std::vector<int> arr3 = {-1, 0, 5, 10, 2, -5, 0};
    std::vector<int> result3 = countFrequencies(arr3, 2);
    assert(result3 == std::vector<int>({2, 0, 1})); // 0 appears twice, 2 once, others ignored

    // n = 0 (only count zeros)
    std::vector<int> arr4 = {0, 0, 0, 1, -1};
    std::vector<int> result4 = countFrequencies(arr4, 0);
    assert(result4 == std::vector<int>({3}));

    // Large n with no matches
    std::vector<int> arr5 = {1, 2, 3};
    std::vector<int> result5 = countFrequencies(arr5, 100);
    assert(result5.size() == 101);
    assert(result5[1] == 1 && result5[2] == 1 && result5[3] == 1);
    // All others should be zero
    for (int i = 0; i <= 100; ++i) {
        if (i != 1 && i != 2 && i != 3) {
            assert(result5[i] == 0);
        }
    }

    std::cout << "All tests passed.\n";
    return 0;
}
