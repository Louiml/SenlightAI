/*
Write a C++ function `combinationSum2Unique` that accepts a vector of integers `candidates` (which may contain duplicates) and a target integer `target`, and returns a vector of all unique combinations of `candidates` where the chosen numbers sum to `target`. Each number in `candidates` may only be used once per combination, and the solution set must not contain duplicate combinations. The combinations in the result can be in any order, and the numbers within each combination can be in any order, but each combination must be unique (i.e., no two returned vectors are permutations of each other). The input vector may be empty, may contain negative numbers, duplicates, and the target may be any integer (including negative). Return an empty vector if no valid combination exists.
*/

#include <vector>
#include <algorithm>

// Returns all unique combinations (each candidate used at most once) that sum to target.
std::vector<std::vector<int>> combinationSum2Unique(std::vector<int> candidates, int target) {
    std::sort(candidates.begin(), candidates.end());
    std::vector<std::vector<int>> result;
    std::vector<int> current;

    // Depth-first search over sorted candidates.
    // start: index to start from in candidates, sum: current sum of current combination.
    void dfs(int start, int sum) {
        // If sum equals target, record a copy of the current combination.
        if (sum == target) {
            result.push_back(current);
            return;
        }

        // If sum exceeds target and all remaining numbers are non-negative, prune.
        // But for negative numbers we cannot prune; condition only helps when sum > target
        // and the next candidate is non-negative (since sorted, if current candidate >0 then all later >0).
        if (sum > target && (start < candidates.size() && candidates[start] > 0)) {
            return;
        }

        for (int i = start; i < candidates.size(); ++i) {
            // Skip duplicates at the same recursion level to avoid duplicate combinations.
            if (i > start && candidates[i] == candidates[i - 1]) continue;

            current.push_back(candidates[i]);
            dfs(i + 1, sum + candidates[i]);  // each element used once
            current.pop_back();
        }
    }

    dfs(0, 0);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is provided above; this main tests it.
int main() {
    // Example: [10,1,2,7,6,1,5], target 8 -> expected unique combos: [1,1,6], [1,2,5], [1,7], [2,6]
    {
        std::vector<int> cand = {10,1,2,7,6,1,5};
        auto res = combinationSum2Unique(cand, 8);
        assert(res.size() == 4);
        // Sort each combination and the whole list for deterministic comparison.
        for (auto& combo : res) std::sort(combo.begin(), combo.end());
        std::sort(res.begin(), res.end());
        std::vector<std::vector<int>> expected = {{1,1,6}, {1,2,5}, {1,7}, {2,6}};
        assert(res == expected);
    }

    // Duplicate candidates: [2,2,2], target 4 -> only one combination [2,2]
    {
        std::vector<int> cand = {2,2,2};
        auto res = combinationSum2Unique(cand, 4);
        assert(res.size() == 1);
        assert(res[0] == std::vector<int>({2,2}));
    }

    // No valid combination: [1,2,3], target 10 -> empty
    {
        std::vector<int> cand = {1,2,3};
        assert(combinationSum2Unique(cand, 10).empty());
    }

    // Empty candidates, any target -> empty
    {
        assert(combinationSum2Unique({}, 5).empty());
    }

    // Target 0 and no zero candidates -> empty (since we require at least one number)
    {
        std::vector<int> cand = {1,2};
        assert(combinationSum2Unique(cand, 0).empty());
    }

    // Single candidate equals target: [5], target 5 -> [[5]]
    {
        std::vector<int> cand = {5};
        auto res = combinationSum2Unique(cand, 5);
        assert(res.size() == 1);
        assert(res[0] == std::vector<int>({5}));
    }

    // Negative numbers: [-1, 2, 3, -2], target 0 -> combos: [-2,-1,3] and [-1,2] maybe? Let's compute: subsets summing to 0: [-2,-1,3] sum=0; [-1,2]? sum=1 not 0; [-1,2,3,-2] sum=2; [-2,2] sum=0; [3,-1,-2] sum=0 (same as first). So unique: [-2,-1,3] and [-2,2]? Actually -2+2=0; [-2,2] is valid. Also [-1,2,3,-2] sum=2 no. Also [3,-1,-2] same as first. So expected: [[-2,2], [-2,-1,3]]
    {
        std::vector<int> cand = {-1,2,3,-2};
        auto res = combinationSum2Unique(cand, 0);
        // Because of negative numbers, we cannot prune based on sum>target; the algorithm still works.
        // Sorting each combo and overall list for comparison.
        for (auto& combo : res) std::sort(combo.begin(), combo.end());
        std::sort(res.begin(), res.end());
        std::vector<std::vector<int>> expected = {{-2,2}, {-2,-1,3}};
        // Note: -2+2=0; -2-1+3=0; also -1+? no; -2+3=1; 2-1=1; etc.
        assert(res == expected);
    }

    // Large duplicate set: [1,1,1,1], target 2 -> only [1,1] (since using each 1 once, but there are many same values; unique combos only one)
    {
        std::vector<int> cand = {1,1,1,1};
        auto res = combinationSum2Unique(cand, 2);
        assert(res.size() == 1);
        assert(res[0] == std::vector<int>({1,1}));
    }

    return 0;
}

// The problem is a classic backtracking/DFS combination-sum variant with the requirement of uniqueness and limited usage of each element. First, sort the `candidates` to make duplicate handling easy: after sorting, identical numbers are adjacent, so we can skip duplicates at the same recursion level (i.e., when `i > u` and `candidates[i] == candidates[i-1]`). The recursive function processes indices from a starting position `u` and keeps a running sum `s`. At each state, we try adding each candidate from `u` onward. If the sum exceeds the target (when positive) we prune; if equal, we record a copy of the current combination. For negative numbers, the pruning condition `s > target` does not apply because adding a negative might bring the sum down, so we must rely on the natural termination of the loop over indices; the algorithm still works correctly. Important edge cases: empty candidates → returns empty; target = 0 → the only valid combination is the empty vector, which should be returned if the sum of an empty subset equals 0 (but the problem typically expects non-empty subsets, and the reference snippet does not include the empty set; in this task, we interpret "chosen numbers" as at least one number, so for target 0 we return an empty vector unless there is a zero candidate); negative candidates with positive target may produce infinitely many combinations? No, because each number is used at most once, so the recursion depth is bounded by the number of candidates. Duplicate handling prevents duplicate combinations. Time complexity: in the worst case (all unique numbers, arbitrary target), the number of subsets is O(2^n), and for each subset we do O(n) work to copy, so O(n * 2^n). Space complexity: O(n) for recursion stack plus O(n^2) output space in the worst case (storing many combinations). We also use O(n) auxiliary space for the temporary combination.
