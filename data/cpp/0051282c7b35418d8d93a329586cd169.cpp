Given an array of \(N\) integers and a length \(M\), write a C++ function `std::vector<std::vector<int>> uniqueCombinations(const std::vector<int>& nums, int m)` that returns all unique **multiset combinations** (i.e., each element can be used at most once, but duplicates in the input are treated as identical) of size exactly \(m\), sorted in lexicographical order. The input may contain duplicate integers, and the output must not contain duplicate combinations. For example, with input `{1,1,2}` and `m=2`, the result should be `{{1,1},{1,2}}` (not `{{1,1},{1,2},{1,2}}`). The returned combinations must be each sorted in ascending order internally, and the overall list sorted lexicographically.
// The problem is to generate all \(m\)-length combinations (unordered selections) from a multiset of size \(N\), while ignoring duplicate combinations caused by identical input elements. The standard backtracking approach: sort the input array so that duplicates are adjacent. Then recursively pick the next element, but when iterating candidates at each recursive level, skip over consecutive duplicate values that have already been considered as the start of a branch (i.e., only take the first occurrence of each value at that level). This avoids generating the same combination multiple times. The recursion builds a combination by choosing one index at a time; once the combination has length `m`, we push a copy into the result. Because we process values in non-decreasing order and never revisit earlier indices (we only move forward), each combination is internally sorted. Duplicates are handled by skipping `if (i > start && nums[i] == nums[i-1]) continue;` in the loop. Edge cases: when `m` is 0, return a single empty combination (or an empty vector depending on interpretation—here we return `{{}}`); when `m > N`, return an empty result. Time complexity: the number of distinct combinations is at most \(C(N,m)\), and for each we do \(O(m)\) work to copy, so worst-case \(O(m \cdot C(N,m))\). Space complexity: \(O(m + K)\) where \(K\) is the number of output combinations, plus recursion stack depth \(m\).
#include <vector>
#include <algorithm>

// Generate all unique combinations (size m) from a multiset of integers.
// Each combination is sorted ascending; the overall result is lexicographically sorted.
std::vector<std::vector<int>> uniqueCombinations(const std::vector<int>& nums, int m) {
    std::vector<std::vector<int>> result;
    if (m < 0 || m > static_cast<int>(nums.size())) {
        return result;
    }
    if (m == 0) {
        result.push_back({});
        return result;
    }

    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    std::vector<int> current;
    current.reserve(m);

    // Backtracking helper: start index for next selection, current combination size.
    auto backtrack = [&](auto&& self, int start, int count) -> void {
        if (count == m) {
            result.push_back(current);
            return;
        }
        for (int i = start; i < static_cast<int>(sorted.size()); ++i) {
            // Skip duplicates: only allow the first occurrence of a value at this level.
            if (i > start && sorted[i] == sorted[i - 1]) {
                continue;
            }
            current.push_back(sorted[i]);
            self(self, i + 1, count + 1);
            current.pop_back();
        }
    };

    backtrack(backtrack, 0, 0);
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared here (or included from the solution).
std::vector<std::vector<int>> uniqueCombinations(const std::vector<int>& nums, int m);

int main() {
    // Basic case with no duplicates.
    std::vector<std::vector<int>> r1 = uniqueCombinations({1,2,3}, 2);
    assert(r1 == std::vector<std::vector<int>>({{1,2},{1,3},{2,3}}));

    // Duplicate input elements: only distinct combinations.
    std::vector<std::vector<int>> r2 = uniqueCombinations({1,1,2}, 2);
    assert(r2 == std::vector<std::vector<int>>({{1,1},{1,2}}));

    // All elements identical.
    std::vector<std::vector<int>> r3 = uniqueCombinations({5,5,5}, 2);
    assert(r3 == std::vector<std::vector<int>>({{5,5}}));

    // m = 0 returns one empty combination.
    std::vector<std::vector<int>> r4 = uniqueCombinations({1,2,3}, 0);
    assert(r4 == std::vector<std::vector<int>>({{}}));

    // m larger than n returns empty.
    std::vector<std::vector<int>> r5 = uniqueCombinations({1,2}, 3);
    assert(r5.empty());

    // Mixed duplicates and larger selection.
    std::vector<std::vector<int>> r6 = uniqueCombinations({2,1,2,1}, 2);
    assert(r6 == std::vector<std::vector<int>>({{1,1},{1,2},{2,2}}));

    // Lexicographic order and internal order.
    std::vector<std::vector<int>> r7 = uniqueCombinations({3,1,2}, 3);
    assert(r7 == std::vector<std::vector<int>>({{1,2,3}}));

    // Larger set with many duplicates.
    std::vector<std::vector<int>> r8 = uniqueCombinations({1,1,1,1}, 3);
    assert(r8 == std::vector<std::vector<int>>({{1,1,1}}));

    // Check that result is sorted lexicographically for a non-trivial case.
    std::vector<std::vector<int>> r9 = uniqueCombinations({1,2,2,3}, 2);
    assert(r9 == std::vector<std::vector<int>>({{1,2},{1,3},{2,2},{2,3}}));

    // Empty input and m=0.
    std::vector<std::vector<int>> r10 = uniqueCombinations({}, 0);
    assert(r10 == std::vector<std::vector<int>>({{}}));

    std::cout << "All tests passed.\n";
    return 0;
}
