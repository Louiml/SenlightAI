Write a C++ function that, given integers `n` and `k` where `1 ≤ k ≤ n ≤ 20`, returns all possible combinations of `k` numbers chosen from the range `[1, n]`, with each combination sorted in ascending order, and the overall list of combinations sorted lexicographically (i.e., by comparing combinations element-by-element, as in standard dictionary order). The function should return a `std::vector<std::vector<int>>`. For example, for `n=4` and `k=2`, the expected result is `{{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}}`. Handle edge cases such as `k=0` (return a single empty combination) and `k=n` (return only `{1,2,...,n}`).

The problem is a classic combinatorial generation task. The optimal approach uses backtracking: recursively build each combination by selecting the next number in increasing order, starting from a current start index. At each recursive step, we maintain a partial combination (path). If the length of `path` equals `k`, we have a valid combination and push it to the result. Otherwise, we iterate from the current start index to `n`, adding each value `i` to `path`, recursing with `start = i + 1` (to avoid duplicates and ensure sorted order), then removing `i` (backtracking) to explore the next possibility. This naturally produces combinations sorted lexicographically because we always pick numbers in increasing order. Edge cases: when `k == 0`, the empty combination is valid and should be returned; the recursion should handle this by checking `path.size() == k` at the start. When `k > n`, no combinations exist (though the task constraints say `k ≤ n`, we can still handle by returning an empty vector). The time complexity is `O(C(n,k) * k)` because we generate each of the `C(n,k)` combinations and copying each takes `O(k)` time. The auxiliary space is `O(k)` for the recursion stack and temporary path, not counting the output storage.

#include <vector>

// Return all combinations of k numbers from 1 to n, sorted lexicographically.
std::vector<std::vector<int>> generateCombinations(int n, int k) {
    std::vector<std::vector<int>> result;
    if (k < 0 || k > n) return result;  // No valid combinations per constraints.
    
    // Helper recursive function using backtracking.
    // Builds combinations by choosing numbers in increasing order.
    auto backtrack = [&](auto&& self, int start, int remaining, std::vector<int>& path) -> void {
        if (remaining == 0) {                         // Complete combination.
            result.push_back(path);
            return;
        }
        // Prune: if there are not enough numbers left to pick, stop.
        if (start > n - remaining + 1) return;
        for (int i = start; i <= n; ++i) {
            path.push_back(i);
            self(self, i + 1, remaining - 1, path);
            path.pop_back();                          // Backtrack.
        }
    };
    
    std::vector<int> path;
    backtrack(backtrack, 1, k, path);
    return result;
}

#include <cassert>
#include <vector>

// Assume generateCombinations is declared above.

int main() {
    // Test 1: Basic n=4, k=2.
    std::vector<std::vector<int>> r1 = generateCombinations(4, 2);
    std::vector<std::vector<int>> e1 = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
    assert(r1 == e1);

    // Test 2: n=1, k=1.
    assert(generateCombinations(1, 1) == std::vector<std::vector<int>>({{1}}));

    // Test 3: n=3, k=3 (full combination).
    assert(generateCombinations(3, 3) == std::vector<std::vector<int>>({{1,2,3}}));

    // Test 4: n=5, k=0 (empty combination).
    assert(generateCombinations(5, 0) == std::vector<std::vector<int>>({{}}));

    // Test 5: k > n (invalid, should return empty).
    assert(generateCombinations(3, 4) == std::vector<std::vector<int>>());

    // Test 6: n=3, k=1.
    assert(generateCombinations(3, 1) == std::vector<std::vector<int>>({{1},{2},{3}}));

    // Test 7: n=4, k=3 (partial combinations).
    std::vector<std::vector<int>> e7 = {{1,2,3},{1,2,4},{1,3,4},{2,3,4}};
    assert(generateCombinations(4, 3) == e7);

    // Test 8: n=6, k=2 (check lexicographic order and size).
    auto r8 = generateCombinations(6, 2);
    assert(r8.size() == 15);
    assert(r8.front() == std::vector<int>({1,2}));
    assert(r8.back() == std::vector<int>({5,6}));

    return 0;
}
