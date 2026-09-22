Write a C++ function named `countClimbingWays` that takes an integer `n` (with `n >= 0`) and a vector `v` of exactly `n+1` non-negative integers, where `v[i]` (for `0 <= i <= n`) represents the maximum number of steps that can be taken from position `i` in a single move. Starting from position `0`, you may move forward by `1` up to `v[i]` steps at any position `i`, but you cannot move beyond position `n`. The goal is to reach exactly position `n`. The function should return the total number of distinct sequences of moves that lead from position `0` to position `n`. If `v[i]` is `0`, no move can be made from position `i`. Note that the order of moves matters (e.g., moving 1 then 2 is different from moving 2 then 1). Use a bottom-up dynamic programming (tabulation) approach to compute the result efficiently, and ensure your function works correctly for cases where `n=0` (return `1` because you are already at the destination) and for very large values (use `long long` to avoid overflow, and assume the answer fits within that type).

The problem is a variation of counting paths in a linear graph where from each position `i`, you have `v[i]` possible forward jumps of lengths `1` to `v[i]`, but you cannot jump past `n`. The recursive definition is: let `ways(i)` be the number of ways to reach `n` from position `i`. The base case is `ways(n) = 1` (already at destination), and for `i < n`, `ways(i) = sum_{j=1}^{v[i]} ways(i+j)` where `i+j <= n`. The recursive solution given in the snippet has a flaw: it loops over `i` from `1` to `n` and then over `j` up to `v[i]`, which is incorrect because it doesn't index `v` properly and does not accumulate from the correct position. The intended logic is that from a given position `i`, you consider all jump lengths `j` from `1` to `v[i]`, but only if `i+j` is within bounds. The bottom-up tabulation works backward from `n` down to `0`: initialize `dp[n] = 1`, and for each `i` from `n-1` down to `0`, sum `dp[i+j]` for `j` in `[1, v[i]]` where `i+j <= n`. This is `O(n * max(v[i]))` time, but since `v[i]` is bounded by `n`, worst-case is `O(n^2)`. Space is `O(n)` for the `dp` array. Important edge cases: `n=0` should return `1` (the function must handle this), and if `v[i]` is `0` for some `i` before `n`, then `dp[i]` remains `0` unless there is a path from `i` that skips over it (which is possible since jumps can go beyond `i`). Also, note that `v` is indexed from `0` to `n`, so ensure the vector has size `n+1`. The function should be `const`-correct by taking `const std::vector<int>&` and `int n`, but since `n` is also the size minus one, it's safer to derive `n` from `v.size()-1` inside the function, or pass `n` and check consistency—we'll accept `n` and `v` such that `v.size() == (size_t)n+1`. Use `long long` for the DP values to avoid overflow.

#include <vector>
#include <cstddef>

// Count the number of distinct sequences to go from position 0 to position n,
// where from each position i you may move forward by 1 to v[i] steps, but never past n.
long long countClimbingWays(int n, const std::vector<int>& v) {
    // Input validation: v must have exactly n+1 elements.
    if (static_cast<int>(v.size()) != n + 1) {
        return 0; // or throw, but for simplicity return 0 on invalid input.
    }

    // dp[i] = number of ways to reach n starting from position i.
    std::vector<long long> dp(n + 1, 0);
    dp[n] = 1; // Base case: already at destination.

    // Fill backwards from n-1 down to 0.
    for (int i = n - 1; i >= 0; --i) {
        int maxJump = v[i];
        // Try all possible jump lengths from 1 to maxJump, but not exceeding n.
        for (int j = 1; j <= maxJump && i + j <= n; ++j) {
            dp[i] += dp[i + j];
        }
    }

    return dp[0];
}

#include <cassert>
#include <vector>

int main() {
    // Case 1: n=0, no moves needed, 1 way.
    std::vector<int> v0 = {0};
    assert(countClimbingWays(0, v0) == 1);

    // Case 2: n=1, v[0]=1, only one move of length 1.
    std::vector<int> v1 = {1, 0};
    assert(countClimbingWays(1, v1) == 1);

    // Case 3: n=2, v[0]=2 (can jump 1 or 2), v[1]=0.
    // Paths: 0->2 (jump 2), 0->1->2 (jump 1 then 1). So 2 ways.
    std::vector<int> v2 = {2, 0, 0};
    assert(countClimbingWays(2, v2) == 2);

    // Case 4: n=3, v = {1,1,0,0}. Only path: 0->1->2 can't go to 3 because v[2]=0, so 0 ways.
    std::vector<int> v3 = {1, 1, 0, 0};
    assert(countClimbingWays(3, v3) == 0);

    // Case 5: n=3, v = {2,2,2,0}. All jumps possible.
    // From 0: jump1 ->1, jump2->2.
    // From 1: jump1->2, jump2->3.
    // From 2: jump1->3.
    // Count: ways(2)=1 (jump to 3), ways(1)=ways(2)+ways(3)=1+1=2, ways(0)=ways(1)+ways(2)=2+1=3.
    std::vector<int> v5 = {2, 2, 2, 0};
    assert(countClimbingWays(3, v5) == 3);

    // Case 6: n=4 with a jump that skips over a zero-move position.
    // v[0]=3, v[1]=0, v[2]=2, v[3]=1, v[4]=0.
    // From 3: to 4 (1 way). From 2: to 3 or 4 -> ways(3)+ways(4)=1+1=2. From 1: no moves (0). From 0: jump1->1 (0), jump2->2 (2), jump3->3 (1) => total 3.
    std::vector<int> v6 = {3, 0, 2, 1, 0};
    assert(countClimbingWays(4, v6) == 3);

    // Large value check: n=5, all v[i]=5, should be 2^(n-1) = 16? Let's compute: ways(5)=1, ways(4)=1, ways(3)=ways(4)+ways(5)=2, ways(2)=ways(3)+ways(4)+ways(5)=2+1+1=4, ways(1)=ways(2)+ways(3)+ways(4)+ways(5)=4+2+1+1=8, ways(0)=ways(1)+ways(2)+ways(3)+ways(4)+ways(5)=8+4+2+1+1=16.
    std::vector<int> v7(6, 5);
    assert(countClimbingWays(5, v7) == 16);

    // Edge: Invalid size returns 0.
    std::vector<int> bad = {1, 2};
    assert(countClimbingWays(3, bad) == 0);
}
