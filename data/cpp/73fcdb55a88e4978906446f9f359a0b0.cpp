/*
Write a C++ function named `findConsecutiveSequence` that takes two integers `N` and `L` as parameters, where `N` is a target sum (0 ≤ N ≤ 10100) and `L` is the minimum allowed sequence length (1 ≤ L ≤ 100). The function must find a sequence of consecutive non-negative integers (starting from 0 or a positive integer) whose sum equals exactly `N`, with the sequence length being at least `L` and at most 100. If multiple valid sequences exist, choose the one with the shortest length; if there is still a tie, choose the one with the smallest starting number. Return the sequence as a `std::vector<int>` sorted in ascending order. If no such sequence exists, return an empty vector. The algorithm must not use brute-force enumeration of all possible sequences; instead, it should efficiently derive the sequence using mathematical properties of arithmetic series.
*/
#include <vector>
#include <cstdint>

// Find the shortest consecutive non-negative integer sequence with sum N and length at least L (max 100).
// Returns empty vector if none exists.
std::vector<int> findConsecutiveSequence(unsigned long long N, int L) {
    // Special case: sum 0 is only achievable by sequence {0} if L <= 1.
    if (N == 0) {
        if (L <= 1) return {0};
        return {};
    }

    // Try lengths from L upwards. Since we want shortest, stop at first success.
    for (int k = L; k <= 100; ++k) {
        // Check if k divides 2N
        if ((2ULL * N) % static_cast<unsigned long long>(k) != 0) continue;
        unsigned long long tmp = (2ULL * N) / static_cast<unsigned long long>(k);
        // The expression for 2a: tmp - k + 1
        if (tmp < static_cast<unsigned long long>(k - 1)) continue; // 2a would be negative
        unsigned long long twoA = tmp - static_cast<unsigned long long>(k - 1);
        if (twoA % 2 != 0) continue; // a must be integer
        unsigned long long a = twoA / 2;
        // a is non-negative by construction. Build the sequence.
        std::vector<int> result;
        result.reserve(k);
        for (int i = 0; i < k; ++i) {
            result.push_back(static_cast<int>(a + static_cast<unsigned long long>(i)));
        }
        return result;
    }
    return {};
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic cases
    assert(findConsecutiveSequence(15, 1) == std::vector<int>({1,2,3,4,5})); // length 5
    assert(findConsecutiveSequence(15, 3) == std::vector<int>({1,2,3,4,5})); // length 5 still shortest
    assert(findConsecutiveSequence(15, 6) == std::vector<int>({})); // no length >=6
    assert(findConsecutiveSequence(0, 1) == std::vector<int>({0}));
    assert(findConsecutiveSequence(0, 2) == std::vector<int>({}));
    assert(findConsecutiveSequence(100, 1) == std::vector<int>({18,19,20,21,22})); // sum=100
    assert(findConsecutiveSequence(100, 5) == std::vector<int>({18,19,20,21,22}));
    assert(findConsecutiveSequence(100, 6) == std::vector<int>({})); // no valid length 6-100? Actually length 5 is max
    // Check length 3 for N=6: 0+1+2+3? sum=6 is length 3 (1+2+3) or length 4 (0+1+2+3). Shortest = 1+2+3
    assert(findConsecutiveSequence(6, 3) == std::vector<int>({1,2,3}));
    // N=2 with L=2: only length 2 is 0+1? sum=1, not 2; length 2 must be a+a+1=2 => a=0.5 no; so none
    assert(findConsecutiveSequence(2, 2) == std::vector<int>({}));
    // N=9 L=2: length 2 gives 4+5=9, also length 3 gives 2+3+4=9, length 4 gives 0+1+2+3+? no, length 5 gives -? So shortest length=2
    assert(findConsecutiveSequence(9, 2) == std::vector<int>({4,5}));
    return 0;
}
// The problem reduces to finding integers `a ≥ 0` and `k` (sequence length) such that:
// `N = a + (a+1) + ... + (a+k-1) = k*(2a + k - 1)/2`.
//
// For each candidate length `k` from `L` up to 100, we can solve for `a`:
// `2N = k*(2a + k - 1)` → `2a = (2N / k) - k + 1`.  
// For `a` to be a non-negative integer, `2N` must be divisible by `k`, and `(2N / k) - k + 1` must be non-negative and even. This gives `a = ((2N / k) - k + 1) / 2`.
//
// We iterate `k` from `L` upwards because we want the shortest length first. Once we find the first valid `k` that yields an integer `a ≥ 0`, we immediately generate the sequence `a, a+1, ..., a+k-1`. If `k > 100` or no valid `k` found, return empty vector. Edge cases: When `N = 0`, the only valid sequence is `{0}` with length 1 if `L ≤ 1`. Also note that sequences must consist of non-negative integers, so `a` can be zero (e.g., `0+1+2+3=6`). The time complexity is O(100 * k) for generating the sequence, effectively O(1) since k ≤ 100; space complexity O(k) for the result.
