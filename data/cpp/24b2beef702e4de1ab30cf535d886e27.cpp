Write a C++ function `std::vector<std::vector<int>> combinationSum2(std::vector<int>& candidates, int target)` that, given a collection of positive integer candidates (which may contain duplicates) and a positive integer target, returns all unique combinations (as vectors of integers sorted in non-decreasing order) where the candidate numbers sum exactly to target. Each number in the input may be used at most once per combination, and the solution set must not contain duplicate combinations (e.g., if the input contains two `1`s, using both is allowed as `[1,1,6]`, but the combination `[1,6]` should appear only once). The returned combinations must each be sorted in non-decreasing order, and the outer vector can be in any order. For example, given `candidates = {10,1,2,7,6,1,5}` and `target = 8`, the function should return four combinations: `{1,1,6}`, `{1,2,5}`, `{1,7}`, and `{2,6}`. Assume all inputs are positive integers and the input vector is non-empty. The function should be efficient and handle duplicate candidates correctly, discarding any combination that exceeds the target early.

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is declared above (or included from the header).
int main() {
    // Example from problem statement.
    std::vector<int> candidates1 = {10,1,2,7,6,1,5};
    auto result1 = combinationSum2(candidates1, 8);
    std::vector<std::vector<int>> expected1 = {{1,1,6}, {1,2,5}, {1,7}, {2,6}};
    // Sort both result and each inner vector for comparison.
    for (auto& comb : result1) std::sort(comb.begin(), comb.end());
    for (auto& comb : expected1) std::sort(comb.begin(), comb.end());
    std::sort(result1.begin(), result1.end());
    std::sort(expected1.begin(), expected1.end());
    assert(result1 == expected1);

    // Single candidate equal to target.
    std::vector<int> candidates2 = {3, 5, 3};
    auto result2 = combinationSum2(candidates2, 3);
    std::vector<std::vector<int>> expected2 = {{3}};
    for (auto& comb : result2) std::sort(comb.begin(), comb.end());
    for (auto& comb : expected2) std::sort(comb.begin(), comb.end());
    std::sort(result2.begin(), result2.end());
    std::sort(expected2.begin(), expected2.end());
    assert(result2 == expected2);

    // No combination possible.
    std::vector<int> candidates3 = {2, 4, 6};
    auto result3 = combinationSum2(candidates3, 5);
    assert(result3.empty());

    // Multiple duplicates produce only unique combinations.
    std::vector<int> candidates4 = {1, 1, 1, 2};
    auto result4 = combinationSum2(candidates4, 3);
    std::vector<std::vector<int>> expected4 = {{1,2}, {1,1,1}};
    for (auto& comb : result4) std::sort(comb.begin(), comb.end());
    for (auto& comb : expected4) std::sort(comb.begin(), comb.end());
    std::sort(result4.begin(), result4.end());
    std::sort(expected4.begin(), expected4.end());
    assert(result4 == expected4);

    // Target smaller than any candidate.
    std::vector<int> candidates5 = {5, 6, 7};
    auto result5 = combinationSum2(candidates5, 4);
    assert(result5.empty());

    // Combination with more than two elements.
    std::vector<int> candidates6 = {1, 2, 3, 4, 5};
    auto result6 = combinationSum2(candidates6, 10);
    std::vector<std::vector<int>> expected6 = {{1,2,3,4}, {1,4,5}, {2,3,5}};
    for (auto& comb : result6) std::sort(comb.begin(), comb.end());
    for (auto& comb : expected6) std::sort(comb.begin(), comb.end());
    std::sort(result6.begin(), result6.end());
    std::sort(expected6.begin(), expected6.end());
    assert(result6 == expected6);

    return 0;
}

#include <vector>
#include <algorithm>

// Find all unique combinations where each candidate is used at most once and the sum equals target.
// The input vector may contain duplicates; each combination is sorted in non-decreasing order.
std::vector<std::vector<int>> combinationSum2(std::vector<int>& candidates, int target) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    std::sort(candidates.begin(), candidates.end());

    // Recursive helper to build combinations.
    // start: index in sorted candidates to consider next.
    // remaining: sum still needed to reach target.
    void backtrack(int start, int remaining) {
        if (remaining == 0) {
            result.push_back(current);
            return;
        }
        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            // Since sorted, if current candidate exceeds remaining, all later ones do too.
            if (candidates[i] > remaining) break;
            // Skip duplicates at the same recursion depth to avoid duplicate combinations.
            if (i > start && candidates[i] == candidates[i - 1]) continue;
            current.push_back(candidates[i]);
            backtrack(i + 1, remaining - candidates[i]);
            current.pop_back();
        }
    }

    backtrack(0, target);
    return result;
}

// The problem is a classic backtracking/DFS combination-finding problem with the constraint that candidates can be used once and duplicates in the input must not produce duplicate combinations in the output. The main algorithm: first sort the input vector in ascending order. Then perform a recursive depth-first search, where at each recursive call we track: the current starting index in the sorted array, the remaining sum needed, the current partial combination, and the result accumulator. At each step, iterate over candidates starting from the provided start index. For each candidate value, if it is greater than the remaining sum, break out of the loop (since sorted, all later values are also too big). To avoid duplicate combinations, skip any candidate value that is equal to the immediately previous candidate value and that previous value is not the starting index of this recursion — this ensures that for a given position in the combination, we use each distinct value only once per recursion level, while still allowing repeated use of the same value if it appears multiple times in the input at different recursion depths. When the remaining sum becomes zero and the current combination is non-empty, add it to the result. Otherwise, recursively call with `start = i + 1` (since each candidate can be used at most once) and `remaining = remaining - candidates[i]`, then pop the added candidate to backtrack. Edge cases: if the target is smaller than the smallest candidate, the result is empty; if there is a candidate equal to the target, it forms a valid single-element combination; duplicates in input must be handled correctly via the skip rule. Time complexity is \(O(2^n)\) in the worst case (exponential, due to subset-sum-like branching), but with pruning by sorted order and the remaining sum constraint it is typically much better; space complexity is \(O(\text{target} / \min(\text{candidate}))\) for recursion depth plus the output size, but in auxiliary terms it is \(O(\text{target})\) in the worst-case recursion stack.
