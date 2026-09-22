/*
Write a C++ function `combinationSum` that takes a vector of distinct positive integers `candidates` and a positive integer `target`, and returns a vector of all unique combinations (as vectors of integers) where the numbers sum to exactly `target`. Each candidate number may be used an unlimited number of times within a single combination. Two combinations are considered different if they differ in the count of at least one number. The order of combinations and the order of numbers inside each combination do not matter. The input guarantees that the total number of valid combinations is fewer than 150. The function should be `const`-correct: it should not modify the input vector, and it should return the result by value.
*/
#include <vector>
#include <algorithm>
#include <functional>

// Return all unique combinations of candidates (with repetition allowed)
// that sum to the given target.
std::vector<std::vector<int>> combinationSum(const std::vector<int>& candidates, int target) {
    std::vector<std::vector<int>> result;
    if (candidates.empty()) return result;

    // Make a local sorted copy to enable pruning and avoid duplicates.
    std::vector<int> sortedCandidates = candidates;
    std::sort(sortedCandidates.begin(), sortedCandidates.end());

    std::vector<int> path;
    // Recursive lambda for backtracking.
    std::function<void(int, int)> backtrack = [&](int start, int remaining) {
        if (remaining == 0) {
            result.push_back(path);
            return;
        }
        for (int i = start; i < sortedCandidates.size(); ++i) {
            int value = sortedCandidates[i];
            // Prune if the candidate exceeds the remaining target.
            if (value > remaining) break;
            path.push_back(value);
            backtrack(i, remaining - value);  // reuse same index for unlimited repetition
            path.pop_back();
        }
    };

    backtrack(0, target);
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Example 1
    std::vector<int> cand1 = {2, 3, 6, 7};
    auto res1 = combinationSum(cand1, 7);
    assert(res1.size() == 2);
    // Verify each combination sums to 7 and is sorted
    for (auto& comb : res1) {
        int sum = 0;
        for (int x : comb) sum += x;
        assert(sum == 7);
        assert(std::is_sorted(comb.begin(), comb.end()));
    }

    // Example 2
    std::vector<int> cand2 = {2, 3, 5};
    auto res2 = combinationSum(cand2, 8);
    assert(res2.size() == 3);
    // Expected: [2,2,2,2], [2,3,3], [3,5]
    std::vector<std::vector<int>> expected2 = {{2,2,2,2}, {2,3,3}, {3,5}};
    // Sort result for comparison
    for (auto& comb : res2) std::sort(comb.begin(), comb.end());
    std::sort(res2.begin(), res2.end());
    std::sort(expected2.begin(), expected2.end());
    assert(res2 == expected2);

    // Example 3
    std::vector<int> cand3 = {2};
    auto res3 = combinationSum(cand3, 1);
    assert(res3.empty());

    // Single candidate that exactly matches target
    std::vector<int> cand4 = {5};
    auto res4 = combinationSum(cand4, 5);
    assert(res4.size() == 1);
    assert(res4[0] == std::vector<int>({5}));

    // Target smaller than all candidates
    std::vector<int> cand5 = {7, 8, 9};
    auto res5 = combinationSum(cand5, 6);
    assert(res5.empty());

    // Multiple combinations with different lengths
    std::vector<int> cand6 = {1, 2};
    auto res6 = combinationSum(cand6, 3);
    assert(res6.size() == 2); // [1,1,1] and [1,2]
    for (auto& comb : res6) {
        int sum = 0;
        for (int x : comb) sum += x;
        assert(sum == 3);
    }

    // Large target with small candidates
    std::vector<int> cand7 = {1};
    auto res7 = combinationSum(cand7, 10);
    assert(res7.size() == 1);
    assert(res7[0].size() == 10);
    assert(std::all_of(res7[0].begin(), res7[0].end(), [](int x){ return x == 1; }));

    return 0;
}
// The problem is a classic backtracking/DFS (depth-first search) problem. We sort the candidates initially to make pruning easier and to ensure combinations are generated in a non-decreasing order, which prevents duplicates. The main recursive function `backtrack` takes the current index `start` to avoid revisiting earlier candidates (which would create permutations of the same combination), the remaining `target`, and the current `path`. At each recursive call:
// - If `target` becomes 0, we have found a valid combination; we push a copy of `path` into the result.
// - If `target` becomes negative, we prune this branch.
// - Otherwise, we iterate over candidates starting from `start` up to the end. For each candidate, we push it onto `path`, subtract it from `target`, and recurse with the same `start` index (because we can reuse the same number unlimited times). After the recursion returns, we pop the element to restore state for the next iteration.
//
// Edge cases: If `candidates` is empty (though constraints say length >= 1), no solution exists. If `target` is smaller than the smallest candidate, the loop will eventually prune all branches because `target - candidate` becomes negative. Duplicate combinations are avoided because we enforce a non-decreasing order by starting the loop from `start` (not 0) and sorting the candidates.
//
// Time complexity: In the worst case, the number of recursive calls is proportional to the number of combinations, which is guaranteed to be less than 150. For each call, we loop over up to N candidates (N <= 30), so the upper bound is O(N * number_of_combinations * depth) but practically it is efficient. Space complexity: O(target / min_candidate) for the recursion stack depth plus O(number_of_combinations * combination_length) for the result storage, which is acceptable given the constraints.
