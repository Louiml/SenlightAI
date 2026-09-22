Write a C++ function `findCombinations` that takes two integers `k` and `n` as input. It should return a vector of vectors of integers, where each inner vector contains exactly `k` distinct integers from 1 to 9 (inclusive), sorted in increasing order, whose sum equals `n`. If no such combination exists, return an empty vector. The function should be `const`-correct (i.e., it does not modify its inputs) and must handle edge cases such as `k=0`, `n` being negative, or `n` being too large to be achievable with `k` digits (maximum sum is 45 for `k=9`). The order of the inner vectors in the result does not matter.

The problem is a classic backtracking/DFS combination problem. We start with the smallest candidate digit (1) and recursively attempt to build combinations by adding the next digit in ascending order, ensuring each digit is used at most once. At each step, we maintain the current combination and the remaining target sum. If the remaining sum becomes negative, we prune the branch. If the remaining sum is exactly zero and the size of the current combination equals `k`, we record it as a valid result. Otherwise, we iterate from the current index up to 9, adding each digit and recursing with the next index. After returning from recursion, we backtrack by removing the last digit. This approach naturally ensures digits are distinct and sorted. Edge cases: if `k` is less than 1 or `n` is less than 1, no valid combination exists because digits are positive and distinct; also, if `k` exceeds 9, it's impossible. The time complexity is \(O(k \cdot \binom{9}{k})\) because we explore all combinations of choosing `k` digits from 9, and each valid combination requires copying `k` elements into the result. The space complexity is \(O(k)\) for the recursion stack and temporary vector, plus the space for the result.

#include <vector>

// Find all combinations of exactly k distinct digits from 1..9 that sum to n.
std::vector<std::vector<int>> findCombinations(int k, int n) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Helper lambda for backtracking.
    void backtrack(int start, int remaining, int depth) {
        // If sum exceeds target, prune.
        if (remaining < 0) return;
        // If we have selected k digits and sum is correct, record.
        if (depth == k) {
            if (remaining == 0) {
                result.push_back(current);
            }
            return;
        }
        // Not enough digits possible to reach k.
        if (9 - start + 1 < k - depth) return;
        
        for (int i = start; i <= 9; ++i) {
            current.push_back(i);
            backtrack(i + 1, remaining - i, depth + 1);
            current.pop_back();
        }
    }
    
    backtrack(1, n, 0);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Helper to check if two vectors are equal ignoring order of inner vectors.
bool sameCombinations(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    auto a_copy = a;
    auto b_copy = b;
    for (auto& v : a_copy) std::sort(v.begin(), v.end());
    for (auto& v : b_copy) std::sort(v.begin(), v.end());
    std::sort(a_copy.begin(), a_copy.end());
    std::sort(b_copy.begin(), b_copy.end());
    return a_copy == b_copy;
}

int main() {
    // Standard case.
    auto res1 = findCombinations(3, 7);
    std::vector<std::vector<int>> expected1 = {{1,2,4}};
    assert(sameCombinations(res1, expected1));

    // Multiple combinations.
    auto res2 = findCombinations(3, 9);
    std::vector<std::vector<int>> expected2 = {{1,2,6},{1,3,5},{2,3,4}};
    assert(sameCombinations(res2, expected2));

    // No valid combination (too high sum).
    auto res3 = findCombinations(2, 20);
    assert(res3.empty());

    // k = 1.
    auto res4 = findCombinations(1, 9);
    std::vector<std::vector<int>> expected4 = {{9}};
    assert(sameCombinations(res4, expected4));

    // n = 0 (not possible with positive digits).
    auto res5 = findCombinations(3, 0);
    assert(res5.empty());

    // Negative n.
    auto res6 = findCombinations(2, -5);
    assert(res6.empty());

    // k larger than 9 (impossible).
    auto res7 = findCombinations(10, 45);
    assert(res7.empty());

    // Max sum with 9 digits.
    auto res8 = findCombinations(9, 45);
    std::vector<std::vector<int>> expected8 = {{1,2,3,4,5,6,7,8,9}};
    assert(sameCombinations(res8, expected8));

    // Sum too small for k positive digits.
    auto res9 = findCombinations(3, 3);
    assert(res9.empty());
}
