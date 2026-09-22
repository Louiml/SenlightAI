// Write a C++ function `int minRemainingOccurrences(const std::vector<int>& nums)` that takes a list of integers (possibly with duplicates) and returns the number of occurrences remaining after repeatedly removing two different values from the list. In each removal operation, you must pick two numbers that have different values, decrement their counts by one each, and remove those two occurrences from the multiset. Continue performing such operations until no two different values exist simultaneously (i.e., at most one distinct value remains). The function should return the total number of leftover occurrences (the sum of counts of the remaining distinct values). If the list is empty, return 0. For example, given `[1,1,2,2,3]`, you can remove pairs like (1,2), (1,3) and be left with a single 2, so return 1. The input may contain negative numbers and zeros, and the list size can be up to 10^5.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minRemainingOccurrences({}) == 0);
    assert(minRemainingOccurrences({5}) == 1);
    assert(minRemainingOccurrences({1,1,1}) == 3); // all same, no pairs

    // Mixed values
    assert(minRemainingOccurrences({1,2}) == 0); // pair them
    assert(minRemainingOccurrences({1,2,3}) == 1); // pair (1,2) leave 3
    assert(minRemainingOccurrences({1,1,2,2,3}) == 1); // as in example

    // Large counts
    assert(minRemainingOccurrences({1,1,1,1,2,2}) == 2); // two 1s left
    assert(minRemainingOccurrences({1,1,1,1,1,2}) == 4); // five 1s vs one 2 -> 4 left

    // Negative and zero values
    assert(minRemainingOccurrences({0,0,-1,-1,-2}) == 1); // pair 0s and -1s, leave -2

    // Stress-like: 1000 ones and 999 twos
    std::vector<int> stress;
    for (int i = 0; i < 1000; ++i) stress.push_back(1);
    for (int i = 0; i < 999; ++i) stress.push_back(2);
    assert(minRemainingOccurrences(stress) == 1);

    return 0;
}
#include <bits/stdc++.h>

// Returns the number of occurrences left after repeatedly removing two different values.
int minRemainingOccurrences(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    std::unordered_map<int, int> freq;
    for (int num : nums) {
        ++freq[num];
    }

    // Max-heap of pairs (count, value). Pair comparison works on first then second.
    std::priority_queue<std::pair<int, int>> pq;
    for (const auto& [value, count] : freq) {
        if (count > 0) {
            pq.push({count, value});
        }
    }

    while (pq.size() > 1) {
        auto [countA, valA] = pq.top();
        pq.pop();
        auto [countB, valB] = pq.top();
        pq.pop();

        --countA;
        --countB;

        if (countA > 0) pq.push({countA, valA});
        if (countB > 0) pq.push({countB, valB});
    }

    if (pq.empty()) return 0;
    return pq.top().first;
}
// The key observation is that the order of removals does not affect the final result. This problem is equivalent to reducing the multiset by repeatedly pairing two different elements. A greedy strategy using a max-heap (priority queue) of counts works: always pick the two values with the largest remaining counts and decrement both by one. This is optimal because pairing the largest counts maximizes the number of successful pairs before a bottleneck occurs. The process continues while at least two distinct values have positive counts. At the end, the only remaining positive count (if any) is the answer. Edge cases include: empty input (return 0), all elements same value (return the total count, since no different pairs exist), and situations where the largest count exceeds the sum of all others—in that case, after all others are exhausted, the leftover is the largest count minus the sum of others. The algorithm runs in O(n log n) time due to heap operations, and uses O(n) space for the heap. The heap is built from a frequency map, so the number of distinct values determines heap size, at most n.
