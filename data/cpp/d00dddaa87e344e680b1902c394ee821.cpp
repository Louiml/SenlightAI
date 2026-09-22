/*
Given an array of integers and a positive integer `k`, write a C++ function that returns a vector containing all elements that appear at least `k` times in the array, sorted in ascending order. The input array may contain duplicates, zeros, and negative numbers. If no element meets the frequency threshold, return an empty vector. The function must not modify the input array and must be efficient for large arrays.
*/
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns all elements appearing at least 'k' times, sorted in ascending order.
std::vector<int> frequentElements(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        ++freq[num];
    }

    std::vector<int> result;
    for (const auto& pair : freq) {
        if (pair.second >= k) {
            result.push_back(pair.first);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above; test with asserts.
int main() {
    // Basic case with duplicates and threshold met.
    assert(frequentElements({1, 2, 2, 3, 3, 3}, 2) == std::vector<int>({2, 3}));
    
    // Threshold 1 returns all distinct elements sorted.
    assert(frequentElements({5, -1, 5, 0}, 1) == std::vector<int>({-1, 0, 5}));
    
    // No element meets threshold.
    assert(frequentElements({1, 2, 3}, 2).empty());
    
    // k larger than array size.
    assert(frequentElements({4, 4}, 5).empty());
    
    // All elements same, k equals size.
    assert(frequentElements({7, 7, 7}, 3) == std::vector<int>({7}));
    
    // Negative and zero values.
    assert(frequentElements({-1, -1, 0, 0, 0, -2}, 2) == std::vector<int>({-1, 0}));
    
    // Empty input array.
    assert(frequentElements({}, 1).empty());
    
    // Exactly threshold for multiple elements.
    assert(frequentElements({1, 1, 2, 2, 3}, 2) == std::vector<int>({1, 2}));
    
    // Large threshold with single value.
    assert(frequentElements({9, 9, 9, 8}, 3) == std::vector<int>({9}));
    
    // Unsorted input array with many duplicates.
    assert(frequentElements({6, 4, 6, 4, 6, 4, 6}, 3) == std::vector<int>({4, 6}));
    
    return 0;
}
// We need to count the frequency of each distinct value in the input array, then collect those values whose frequency is at least `k`. The straightforward approach is to use an unordered map (hash map) to store value-to-count associations. Iterate through the array once, incrementing the count for each element. Then iterate through the map, selecting keys whose count meets or exceeds `k`. Finally, sort the selected values in ascending order using `std::sort` before returning. Edge cases: empty input array (return empty), `k` larger than the array size (return empty), all elements unique with `k > 1` (return empty), and cases where the threshold is exactly met (e.g., `k = 1` includes all unique elements). Time complexity is O(n + m log m) where `n` is the array size and `m` is the number of distinct elements satisfying the threshold; space complexity is O(d) for the hash map, where `d` is the number of distinct elements in the input.
