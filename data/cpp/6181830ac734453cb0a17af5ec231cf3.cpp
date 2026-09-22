/*
Write a C++ function `combination_sum_no_repeat` that takes a vector of integers `candidates` and a target integer `target`, and returns a vector of vectors containing all unique combinations where the numbers sum to `target`. Each number in `candidates` may be used only once per combination. The result must not contain duplicate combinations, and combinations can be in any order internally or externally. The input vector may contain duplicate values, and the function must handle them correctly by not producing duplicate combinations (e.g., if `candidates = [1,1,2]` and `target = 3`, only `[1,2]` should appear once, not twice). Assume `candidates` has length between 1 and 100, each value between 1 and 50, and `target` between 1 and 30. The function should return an empty vector if no combination exists.
*/
#include <vector>
#include <algorithm>

// Return all unique combinations of candidates (each used at most once) that sum to target.
std::vector<std::vector<int>> combination_sum_no_repeat(std::vector<int>& candidates, int target) {
    std::sort(candidates.begin(), candidates.end());
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Recursive helper to build combinations.
    void backtrack(int start, int remaining) {
        if (remaining == 0) {
            result.push_back(current);
            return;
        }
        if (start >= static_cast<int>(candidates.size()) || remaining < 0) return;
        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            // Skip duplicates at same recursion level.
            if (i > start && candidates[i] == candidates[i - 1]) continue;
            // Early pruning: if remaining < current value, further values are larger due to sorting.
            if (candidates[i] > remaining) break;
            current.push_back(candidates[i]);
            backtrack(i + 1, remaining - candidates[i]);
            current.pop_back();
        }
    }
    
    backtrack(0, target);
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is assumed to be defined above.

int main() {
    // Example 1 from problem.
    std::vector<int> cand1 = {10,1,2,7,6,1,5};
    auto res1 = combination_sum_no_repeat(cand1, 8);
    std::vector<std::vector<int>> expected1 = {{1,1,6},{1,2,5},{1,7},{2,6}};
    assert(res1.size() == expected1.size());
    for (size_t i = 0; i < expected1.size(); ++i) {
        assert(res1[i] == expected1[i]);
    }

    // Example 2.
    std::vector<int> cand2 = {2,5,2,1,2};
    auto res2 = combination_sum_no_repeat(cand2, 5);
    std::vector<std::vector<int>> expected2 = {{1,2,2},{5}};
    assert(res2.size() == expected2.size());
    for (size_t i = 0; i < expected2.size(); ++i) {
        assert(res2[i] == expected2[i]);
    }

    // No combination possible.
    std::vector<int> cand3 = {3,4,5};
    auto res3 = combination_sum_no_repeat(cand3, 1);
    assert(res3.empty());

    // Single element exactly equals target.
    std::vector<int> cand4 = {7};
    auto res4 = combination_sum_no_repeat(cand4, 7);
    assert(res4.size() == 1 && res4[0] == std::vector<int>{7});

    // Duplicates but only one unique combination.
    std::vector<int> cand5 = {1,1,2};
    auto res5 = combination_sum_no_repeat(cand5, 3);
    assert(res5.size() == 1 && res5[0] == std::vector<int>{1,2});

    // All elements used once.
    std::vector<int> cand6 = {1,2,3};
    auto res6 = combination_sum_no_repeat(cand6, 6);
    assert(res6.size() == 1 && res6[0] == std::vector<int>{1,2,3});

    // Target larger than sum of all elements.
    std::vector<int> cand7 = {1,1,1};
    auto res7 = combination_sum_no_repeat(cand7, 4);
    assert(res7.empty());

    // Multiple combinations with duplicates.
    std::vector<int> cand8 = {1,1,1,2,2,3};
    auto res8 = combination_sum_no_repeat(cand8, 4);
    std::vector<std::vector<int>> expected8 = {{1,1,2},{1,3},{2,2}}; // order may vary, check size and content set
    assert(res8.size() == expected8.size());
    for (const auto& comb : expected8) {
        assert(std::find(res8.begin(), res8.end(), comb) != res8.end());
    }

    return 0;
}
// The solution uses a backtracking (depth-first search) approach. First, sort the input array to group identical values together, which simplifies duplicate removal. The recursive function explores combinations starting from a given index, maintaining a current partial combination and the remaining target sum. At each step, iterate from the start index to the end; skip any value that equals the previous one in the current iteration level (i.e., `i > start && candidates[i] == candidates[i-1]`) to avoid generating duplicate combinations. If the remaining target becomes zero, a valid combination is found and added to the result. If the remaining target becomes negative or the start index exceeds the array size, the branch is pruned. The recursion advances the start index to `i+1` because each number can be used only once. Time complexity is `O(2^n)` in the worst case (though pruning helps), and space complexity is `O(n)` for the recursion stack plus the output storage.
