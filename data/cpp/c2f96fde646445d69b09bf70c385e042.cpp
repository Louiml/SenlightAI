Write a C++ function that takes an array of integers and its size, and returns the smallest positive integer (greater than 0) that is missing from the array. The array may contain negative numbers, zeros, duplicates, and unordered elements. The function should handle edge cases like arrays with no positive numbers, arrays starting from 1 with no gaps, and arrays containing very large values. The solution must work in-place without modifying the original array contents (i.e., it may sort a copy if needed) and must not use extra data structures beyond a constant amount of auxiliary space (excluding the copy if used).

#include <cassert>
#include <vector>

// (The solution function is declared above. This file includes it directly for testing.)
int main() {
    // Basic cases
    assert(smallestMissingPositive({1, 2, 0}) == 3);
    assert(smallestMissingPositive({3, 4, -1, 1}) == 2);
    assert(smallestMissingPositive({7, 8, 9, 11, 12}) == 1);

    // Arrays with negatives and zeros only
    assert(smallestMissingPositive({-1, -2, 0}) == 1);
    assert(smallestMissingPositive({0, 0, 0}) == 1);
    assert(smallestMissingPositive({-5, -3, -2}) == 1);

    // Consecutive from 1 with no gap
    assert(smallestMissingPositive({1, 2, 3, 4}) == 5);
    assert(smallestMissingPositive({1}) == 2);

    // Duplicates and unsorted
    assert(smallestMissingPositive({1, 1, 1}) == 2);
    assert(smallestMissingPositive({2, 2, 2, 1}) == 3);
    assert(smallestMissingPositive({5, 5, 5, 5, 1, 2, 3}) == 4);

    // Large values and negative mixed
    assert(smallestMissingPositive({100, 200, 1, -1}) == 2);
    assert(smallestMissingPositive({-10, 1, 2, 3, 4, 5, 6}) == 7);

    // Empty array (if permitted, but our function returns 1)
    assert(smallestMissingPositive({}) == 1);
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the smallest positive integer missing from the given array.
// The original array is not modified (a copy is sorted internally).
int smallestMissingPositive(const std::vector<int>& arr) {
    if (arr.empty()) {
        return 1;
    }
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    int candidate = 1;
    for (int value : sorted) {
        if (value == candidate) {
            ++candidate;
        }
    }
    return candidate;
}

// The simplest correct approach is to sort the array and then scan it while maintaining a candidate answer starting at 1. After sorting, we iterate through the elements; whenever we encounter the current candidate value, we increment it. This works because sorting arranges all duplicates and non-positive numbers together—negative and zero values are simply ignored since they are never equal to a positive candidate. The first positive integer that is not found consecutively in the sorted sequence is the answer. Edge cases: if the array contains only negatives and zeros, the candidate remains 1 (correct). If the array contains 1,2,3 but missing 4, the scan stops at 4. Duplicates are harmless because we only increment when we see an exact match. Sorting takes O(n log n) time and the scan takes O(n), so overall O(n log n) time. Space is O(1) auxiliary if we sort in-place, but since the task says "in-place without modifying the original array", we sort a copy which uses O(n) extra space. For a self-contained exercise, we can note that both are acceptable; the reference solution uses a copy to preserve input integrity.
