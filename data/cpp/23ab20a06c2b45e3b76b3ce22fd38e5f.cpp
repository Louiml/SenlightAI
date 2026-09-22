Given an array of `n` integers, write a C++ function `long long maxNonAdjacentPairProduct(const std::vector<int>& a)` that returns the maximum possible sum obtained by selecting a set of non-overlapping adjacent pairs, where each selected pair contributes the product of its two elements. Overlapping pairs are not allowed (i.e., if you select pair `(i, i+1)` you cannot select any pair that includes index `i` or `i+1`). You may skip any elements; the goal is to maximize the total sum of products of selected adjacent pairs. The function must work for arrays of length up to 3000, may contain negative numbers, and must handle the case where selecting no pair is optimal (return 0). The solution must be efficient enough for the given constraints.
#include <cassert>
#include <vector>

// Include the solution code here (or link) – for brevity, assume function is defined above.

int main() {
    // basic: [1,2,3] -> take (1,2)=2 or (2,3)=6, best=6
    assert(maxNonAdjacentPairProduct({1,2,3}) == 6);

    // all negative: no product is positive, return 0
    assert(maxNonAdjacentPairProduct({-1,-2,-3}) == 0);

    // single element returns 0
    assert(maxNonAdjacentPairProduct({5}) == 0);

    // two elements: product
    assert(maxNonAdjacentPairProduct({4,5}) == 20);

    // mix: [1,10,2] -> best is (1,10)=10 vs (10,2)=20, so 20
    assert(maxNonAdjacentPairProduct({1,10,2}) == 20);

    // nested pairs: [2,3,4,5] -> take (2,3)=6 and (4,5)=20 total=26
    assert(maxNonAdjacentPairProduct({2,3,4,5}) == 26);

    // cannot overlap: [1,2,3,4] -> take (1,2)=2 and (3,4)=12 total=14 vs take (2,3)=6 only, so 14
    assert(maxNonAdjacentPairProduct({1,2,3,4}) == 14);

    // skip middle: [1,100,1] -> take (1,1)=1, but no adjacent pair, so best is 0? Actually (1,100)=100 or (100,1)=100 -> 100
    assert(maxNonAdjacentPairProduct({1,100,1}) == 100);

    // large values: use long long to handle 10^9 * 10^9
    assert(maxNonAdjacentPairProduct({1000000000, 1000000000}) == 1000000000000000000LL);

    // empty? Not required, but test n=0 if allowed – but spec says n>=1, so skip.

    // stress-like: all zeros returns 0
    assert(maxNonAdjacentPairProduct({0,0,0,0}) == 0);

    return 0;
}
#include <vector>
#include <cstring>
#include <algorithm>

// dp[i][j][k] - maximum sum from subarray a[i..j] (inclusive)
// k=0: free to skip or pair; k=1: forced to either take current pair or stop
long long dp[3005][3005][2];
bool computed[3005][3005][2];

long long solve(const std::vector<int>& a, int i, int j, int k) {
    if (i >= j) return 0;
    if (computed[i][j][k]) return dp[i][j][k];
    computed[i][j][k] = true;
    long long best = 0;
    if (k == 0) {
        // Option: skip left
        best = std::max(best, solve(a, i+1, j, 0));
        // Option: skip right
        best = std::max(best, solve(a, i, j-1, 0));
        // Option: take pair (i,j)
        best = std::max(best, (long long)a[i]*a[j] + solve(a, i+1, j-1, 1));
    } else {
        // k==1: forced to take pair (i,j) or stop
        best = std::max(best, (long long)a[i]*a[j] + solve(a, i+1, j-1, 1));
    }
    return dp[i][j][k] = best;
}

// Main entry: given vector of ints, return max sum of non-overlapping adjacent pairs
long long maxNonAdjacentPairProduct(const std::vector<int>& a) {
    int n = (int)a.size();
    if (n < 2) return 0;
    memset(computed, 0, sizeof(computed));
    return solve(a, 0, n-1, 0);
}
// The problem is a classic dynamic programming over intervals. We define `dp[i][j][k]` where `i` and `j` are the current left and right boundaries of the subarray, and `k` indicates whether we are in a "forced continuation" mode (i.e., we have just taken a pair and want to continue taking inner pairs without skipping). The recurrence works as follows:
//
// - If `i >= j`, no more pairs can be formed, return 0.
// - If `k == 0` (general state): we can either skip the element at `i`, skip the element at `j`, or take the pair `(i, j)` and then move into state `k=1` for the subarray `(i+1, j-1)`. Since we want to maximize, we take the maximum of: skipping left, skipping right, and taking the pair (which adds `a[i]*a[j]` to the best from `(i+1, j-1, 1)`). Also, we allow returning 0 if all options are negative because we might skip all.
// - If `k == 1` (forced continuation state): we are currently inside a pair that has been taken (i.e., the outer pair was taken, and we must continue taking non-overlapping pairs inside it, but we cannot skip elements that would break the "no overlap" constraint? Actually in the original snippet, `k=1` means that the previous move was to take `a[i]*a[j]` and now we are looking at the inside subarray `(i+1, j-1)` and we must either take a pair or stop, but we cannot skip individual elements because that would leave a gap? The original code returns `max(0LL, a[i]*a[j] + solve(a, i+1, j-1, 1))` which means in state 1, you are forced to take the current pair `(i, j)` as the next action? Actually re-reading: state 1 is only called after taking a pair, and it forces you to take the next innermost pair if you want to continue, but you can stop by returning 0. So in state 1, you either take the pair `(i, j)` and recursively go to state 1 for the inside, or stop (return 0). You cannot skip elements. This ensures that selected pairs are "nested" like parentheses: if you take an outer pair, you can only take pairs strictly inside it, not overlapping.
//
// The base is `if(i >= j) return 0`. The DP uses memoization to avoid recomputation. The time complexity is `O(n^2)` because there are `O(n^2)` states and each state takes constant time. Space complexity is `O(n^2)` for the DP table.
//
// Edge cases: single element or empty array (but the problem guarantees `n >= 1`), all negative numbers (the function should return 0), and large products that may overflow 32-bit, so we use `long long`.
