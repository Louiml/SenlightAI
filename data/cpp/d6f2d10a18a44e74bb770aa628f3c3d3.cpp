// Write a C++ function `combinationSum3(int k, int n)` that returns all combinations of exactly `k` distinct digits from 1 to 9 (inclusive, each used at most once) whose sum equals `n`. The result should be a `std::vector<std::vector<int>>` where each inner vector contains `k` integers in ascending order. No duplicate combinations are allowed, and the order of combinations in the outer vector does not matter. Assume `k` is between 1 and 9, and `n` is a positive integer. If no valid combination exists, return an empty vector. The function must be free-standing (no class), use proper `const` correctness where applicable, and include all necessary headers.
// The problem is a classic backtracking/DFS combination search. We recursively build combinations by selecting digits in increasing order starting from `startIndex` (initially 1) to avoid duplicates and premature ordering. At each step, we maintain a current path (selected digits) and a running sum. We apply two pruning strategies: (1) if the running sum exceeds `n`, we stop exploring this branch because all digits are positive, so the sum can only increase; (2) in the loop, we only iterate up to a digit such that there are still enough remaining digits to reach length `k` (i.e., `9 - i + 1 >= k - path.size()`). When the path length equals `k`, we check if the sum equals `n`; if so, we record the path. The recursion explores all valid choices and backtracks by removing the last digit and subtracting its value. Time complexity: In the worst case (unbounded k and n) the number of combinations is C(9,k), which is at most 2^9 = 512, but due to pruning this is efficient. More precisely, we generate all subsets of {1..9} of size k, and for each we check its sum; thus O(C(9,k) * k) time for building and copying vectors, and O(k) stack space plus O(k) for each path vector stored in the result (result space O(C(9,k)*k)). Edge cases: k > 9 returns empty; n very large returns empty; n very small (less than sum of 1..k) returns empty; duplicates not possible due to distinct digits and increasing order.
#include <vector>

// Return all combinations of exactly k distinct digits from 1..9 summing to n.
std::vector<std::vector<int>> combinationSum3(int k, int n) {
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    
    // Helper lambda for backtracking; captures result and path by reference.
    // target: remaining sum we need to achieve; start: smallest digit to try next.
    auto backtrack = [&](auto&& self, int target, int start) -> void {
        // Prune: if target negative, no need to continue (digits positive).
        if (target < 0) return;
        // Prune: if current path already has k elements, check if sum matches.
        if (path.size() == static_cast<size_t>(k)) {
            if (target == 0) {
                result.push_back(path);
            }
            return;
        }
        // Enough digits left? start..9 must contain at least k - path.size() numbers.
        // Loop only while we can still fill the remaining slots.
        int maxStart = 9 - (k - static_cast<int>(path.size())) + 1;
        for (int i = start; i <= maxStart; ++i) {
            path.push_back(i);
            self(self, target - i, i + 1);
            path.pop_back();
        }
    };
    
    backtrack(backtrack, n, 1);
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Forward declaration of the solution function (already defined).
// Here we only test it.

int main() {
    // k=3, n=7 -> only combination is {1,2,4}
    auto res1 = combinationSum3(3, 7);
    assert(res1.size() == 1);
    assert(res1[0] == std::vector<int>({1,2,4}));

    // k=3, n=9 -> combinations: {1,2,6}, {1,3,5}, {2,3,4}
    auto res2 = combinationSum3(3, 9);
    assert(res2.size() == 3);
    std::vector<std::vector<int>> expected2 = {{1,2,6},{1,3,5},{2,3,4}};
    for (size_t i = 0; i < expected2.size(); ++i) {
        assert(std::find(res2.begin(), res2.end(), expected2[i]) != res2.end());
    }

    // k=4, n=1 -> impossible (min sum 1+2+3+4=10)
    assert(combinationSum3(4, 1).empty());

    // k=1, n=5 -> {5}
    auto res3 = combinationSum3(1, 5);
    assert(res3.size() == 1);
    assert(res3[0] == std::vector<int>({5}));

    // k=9, n=45 -> only {1,2,3,4,5,6,7,8,9}
    auto res4 = combinationSum3(9, 45);
    assert(res4.size() == 1);
    assert(res4[0] == std::vector<int>({1,2,3,4,5,6,7,8,9}));

    // k=5, n=18 -> a known combination exists; check size and sums
    auto res5 = combinationSum3(5, 18);
    assert(!res5.empty());
    for (const auto& vec : res5) {
        assert(vec.size() == 5);
        int sum = 0;
        for (int x : vec) sum += x;
        assert(sum == 18);
        // ensure distinct and ascending
        for (size_t i = 1; i < vec.size(); ++i) {
            assert(vec[i] > vec[i-1]);
        }
    }

    // k=2, n=17 -> {8,9} only
    auto res6 = combinationSum3(2, 17);
    assert(res6.size() == 1);
    assert(res6[0] == std::vector<int>({8,9}));

    // k=3, n=30 -> impossible (max 7+8+9=24)
    assert(combinationSum3(3, 30).empty());

    // k=0 (though constraints say k>=1, we can still handle gracefully) -> empty
    // But since specification says k between 1 and 9, we don't test invalid input.

    return 0;
}
