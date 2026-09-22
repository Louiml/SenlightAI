// Given a non-empty vector of 32-bit integers and a positive integer `k` that is guaranteed to be no larger than the number of distinct elements in the vector, write a C++ function that returns a vector containing the `k` most frequent distinct integers from the input. The frequency of an integer is the number of times it appears in the input. The returned vector may be in any order, but must contain exactly the `k` integers with the highest frequencies. If two or more integers have the same frequency, any of them may be chosen to fill the remaining positions, as long as the total set of returned integers corresponds to the `k` highest frequency values (with ties broken arbitrarily). The function must handle negative integers, zeros, and large counts correctly, and must not modify the input vector.

#include <vector>
#include <cassert>
#include <algorithm>

// The solution function is assumed to be declared above.

int main() {
    // Basic case: distinct frequencies.
    {
        std::vector<int> nums = {1, 1, 1, 2, 2, 3};
        std::vector<int> result = topKFrequent(nums, 2);
        std::vector<int> expected = {1, 2};
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // All identical elements: only one distinct, k must be 1.
    {
        std::vector<int> nums = {7, 7, 7, 7};
        std::vector<int> result = topKFrequent(nums, 1);
        assert(result.size() == 1 && result[0] == 7);
    }

    // Negative numbers and large counts.
    {
        std::vector<int> nums = {-3, -3, -3, -1, -1, 0, 5, 5, 5, 5};
        std::vector<int> result = topKFrequent(nums, 3);
        std::vector<int> expected = {-3, 5, -1}; // Frequencies: 5→4, -3→3, -1→2
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // k equals the number of distinct elements.
    {
        std::vector<int> nums = {1, 2, 3, 4};
        std::vector<int> result = topKFrequent(nums, 4);
        std::vector<int> expected = {1, 2, 3, 4};
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Tie in frequency: any of the tied elements are acceptable.
    {
        std::vector<int> nums = {1, 1, 2, 2, 3, 3};
        std::vector<int> result = topKFrequent(nums, 1);
        // Since all frequencies are 2, the result must contain exactly one of {1,2,3}
        assert(result.size() == 1 && (result[0] == 1 || result[0] == 2 || result[0] == 3));
    }

    // Single element vector.
    {
        std::vector<int> nums = {42};
        std::vector<int> result = topKFrequent(nums, 1);
        assert(result.size() == 1 && result[0] == 42);
    }

    // Large input to verify no overflow or performance issue.
    {
        std::vector<int> nums;
        for (int i = 0; i < 1000; ++i) nums.push_back(i % 10);
        std::vector<int> result = topKFrequent(nums, 3);
        // Frequencies: each digit 0-9 appears 100 times, so any three digits are valid.
        assert(result.size() == 3);
        // Check that all result values are within 0-9.
        for (int val : result) {
            assert(val >= 0 && val < 10);
        }
    }

    return 0;
}

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

// Returns a vector containing the k most frequent integers from the input.
// Frequencies are counted, and a min-heap of size k keeps the highest counts.
// The result vector's order is arbitrary.
std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
    // Count frequencies of each integer.
    std::unordered_map<int, int> frequency;
    for (int value : nums) {
        ++frequency[value];
    }

    // Min-heap storing {frequency, integer} pairs, ordered by frequency first.
    using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> minHeap;

    // Process each distinct integer and keep only the k highest frequencies.
    for (const auto& entry : frequency) {
        minHeap.push({entry.second, entry.first});
        if (minHeap.size() > k) {
            minHeap.pop(); // Remove the current smallest frequency.
        }
    }

    // Extract the integers from the heap into the result vector.
    std::vector<int> result;
    result.reserve(k);
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }

    return result;
}

// The solution first builds a frequency map (unordered_map) by iterating over the entire input vector and incrementing the count for each integer; this takes O(n) time where n is the number of elements in the input. Then, to find the `k` most frequent integers, we use a min-heap (priority_queue with `greater` comparator) that stores pairs of `{frequency, integer}`. We iterate over each unique entry in the frequency map, push the pair into the heap, and if the heap size exceeds `k`, we pop the smallest frequency element (the top of the min-heap). This ensures the heap always contains the `k` largest frequencies seen so far. After processing all unique entries, the heap contains exactly the desired `k` integers (with their counts), and we extract them one by one into the result vector. Edge cases include when `k` equals the number of distinct integers (no pops needed), when all integers are identical (heap size stays 1), and when frequencies tie (the heap ordering by frequency only, with the frequency as the first element of the pair, will keep a deterministic selection based on the integer value for ties, but any tie-breaking is acceptable). Time complexity is O(n + m log k) where m is the number of distinct integers (m ≤ n), and space complexity is O(m + k) for the frequency map and the heap.
