Write a C++ function `long long pancakeValueSum(int S, const std::vector<long long>& pancakes)` that, given a sequence of pancake stack sizes (each between 0 and 1e9), computes the sum over all contiguous subarrays of the product of the minimum element in that subarray and the length of that subarray, modulo 1,000,000,007. For each subarray defined by indices `L` to `R` (0-indexed, inclusive), let `m = min(pancakes[L..R])` and `len = R - L + 1`; add `m * len` to the total. Return the total modulo 1,000,000,007. The function must handle up to 1,000,000 elements efficiently, including cases with duplicate values, increasing or decreasing sequences, and all elements equal. You must not use sorting or a segment tree; instead, use a monotonic stack or a divide-and-conquer approach. Ensure the function is robust for large inputs and performs within time limits.

// The key insight is to compute the contribution of each element as the minimum of some subarrays. For a given element at position `i` with value `v = pancakes[i]`, we need to find the number of subarrays where it is the minimum (or one of the minima, handling duplicates carefully to avoid double counting). A common efficient approach is to use a monotonic stack to compute for each index `i` the nearest index to the left `L[i]` that is strictly less than `v` (or -1 if none) and the nearest index to the right `R[i]` that is less than or equal to `v` (or `S` if none). This ensures each subarray is counted exactly once: for each `i`, any subarray that has its minimum at `i` will have left boundary in `(L[i], i]` and right boundary in `[i, R[i])`. The number of such subarrays is `(i - L[i]) * (R[i] - i)`. The contribution of `v` is `v * (i - L[i]) * (R[i] - i)`. Summing this over all `i` gives the answer. However, this direct count multiplies `v` by the count of subarrays, but the problem requires multiplying the minimum by the length of each subarray, not just the count. To incorporate the length, we need to sum over all subarrays where `i` is the minimum: for each left boundary `l` in `(L[i], i]` and right boundary `r` in `[i, R[i])`, the subarray length is `r - l + 1`. The sum of lengths over all such `(l, r)` pairs can be computed using prefix sums. Specifically, for a fixed `i`, the sum of `(r - l + 1)` over `l` in `[a, i]` and `r` in `[i, b]` (where `a = L[i]+1`, `b = R[i]-1`) equals: (number of left choices) * (sum of right indices) - (sum of left indices) * (number of right choices) + (number of left choices) * (number of right choices) * (1 - i + i? Wait, better to derive: each pair contributes `r - l + 1`. Sum over all pairs = (sum_{l=a}^{i} (i - l + 1)) * (b - i + 1) + (sum_{r=i}^{b} (r - i)) * (i - a + 1)? Actually, the total sum of lengths = Σ_{l=a}^{i} Σ_{r=i}^{b} (r - l + 1) = (b-i+1) * Σ_{l=a}^{i} (i - l + 1) + (i-a+1) * Σ_{r=i}^{b} (r - i). Because `r - l + 1 = (r - i) + (i - l) + 1`. Then sum over pairs = (number of right choices) * Σ_{l} (i - l + 1) + (number of left choices) * Σ_{r} (r - i) + (number of left choices)*(number of right choices)*1. Using arithmetic series, Σ_{l=a}^{i} (i - l + 1) = (i - a + 1)(i - a + 2)/2, and Σ_{r=i}^{b} (r - i) = (b - i)(b - i + 1)/2. So the sum of lengths for that `i` is `(b-i+1) * ((i-a+1)(i-a+2)/2) + (i-a+1) * ((b-i)(b-i+1)/2) + (i-a+1)*(b-i+1)`. To avoid overflow, compute modulo MOD with careful modular arithmetic (since MOD is prime but we only need modulo). Then multiply by `v` and add to total. Edge cases: duplicates must be handled by using strict < on left and <= on right (or vice versa) to ensure each subarray's minimum is uniquely assigned to the leftmost occurrence. Complexity: O(S) time and O(S) space for the monotonic stack and arrays. The function should handle S up to 1e6, so iterative implementation is necessary.

#include <vector>
#include <stack>

const long long MOD = 1000000007LL;

// Compute sum over all subarrays of (min * length) modulo MOD.
// Uses monotonic stack to find left/right boundaries for each element as the unique minimum.
long long pancakeValueSum(int S, const std::vector<long long>& pancakes) {
    if (S == 0) return 0;

    std::vector<int> left(S), right(S);
    std::stack<int> st;

    // Left boundary: nearest index with value < pancakes[i] (strictly less)
    for (int i = 0; i < S; ++i) {
        while (!st.empty() && pancakes[st.top()] >= pancakes[i]) st.pop();
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    while (!st.empty()) st.pop();

    // Right boundary: nearest index with value <= pancakes[i] (less or equal)
    // This makes each subarray's minimum assigned to the leftmost minimum.
    for (int i = S - 1; i >= 0; --i) {
        while (!st.empty() && pancakes[st.top()] > pancakes[i]) st.pop();
        right[i] = st.empty() ? S : st.top();
        st.push(i);
    }

    // Helper to compute sum of arithmetic series modulo MOD.
    auto sum1toN = [](long long n) -> long long {
        if (n <= 0) return 0;
        return (n % MOD) * ((n + 1) % MOD) % MOD * 500000004LL % MOD; // inverse of 2 mod MOD
    };

    long long total = 0;

    for (int i = 0; i < S; ++i) {
        long long v = pancakes[i] % MOD;
        int a = left[i] + 1; // leftmost possible boundary
        int b = right[i] - 1; // rightmost possible boundary
        long long leftChoices = i - a + 1;
        long long rightChoices = b - i + 1;

        // Sum of (i - l + 1) for l in [a, i]
        long long sumLeftPart = (leftChoices % MOD) * ((leftChoices + 1) % MOD) % MOD * 500000004LL % MOD;

        // Sum of (r - i) for r in [i, b]
        long long sumRightPart = (rightChoices - 1) % MOD;
        long long sumRight = (sumRightPart * (sumRightPart + 1) % MOD * 500000004LL % MOD);

        // Sum of lengths over all (l, r) pairs
        long long lengthSum = (rightChoices % MOD) * sumLeftPart % MOD;
        lengthSum = (lengthSum + (leftChoices % MOD) * sumRight % MOD) % MOD;
        lengthSum = (lengthSum + (leftChoices % MOD) * (rightChoices % MOD)) % MOD;

        total = (total + v % MOD * lengthSum) % MOD;
    }

    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(pancakeValueSum(1, {5}) == 5); // subarray [5] min=5 len=1 -> 5
    assert(pancakeValueSum(2, {1, 2}) == 1*1 + 1*2 + 2*1 = 1+2+2=5); // subarrays: [1] min1 len1=1, [2] min2 len1=2, [1,2] min1 len2=2 -> total 5
    assert(pancakeValueSum(3, {3, 1, 2}) == ? actually compute: subarrays: [3] 3, [1]1, [2]2, [3,1] min1 len2=2, [1,2] min1 len2=2, [3,1,2] min1 len3=3 -> total 3+1+2+2+2+3=13
    assert(pancakeValueSum(3, {3,1,2}) == 13);
    // All equal
    assert(pancakeValueSum(3, {2,2,2}) == 2*(1+2+3)*3? Actually sum over all subarrays: lengths 1:3 subarrays each length1 -> 2*1*3=6; length2:2 subarrays each length2 -> 2*2*2=8; length3:1 subarray length3 ->2*3=6; total=20)
    assert(pancakeValueSum(3, {2,2,2}) == 20);
    // Decreasing
    assert(pancakeValueSum(3, {5,4,3}) == subarrays: [5]=5, [4]=4, [3]=3, [5,4] min4 len2=8, [4,3] min3 len2=6, [5,4,3] min3 len3=9 -> total 5+4+3+8+6+9=35
    assert(pancakeValueSum(3, {5,4,3}) == 35);
    // Large value mod
    assert(pancakeValueSum(1, {1000000007LL}) == 0); // 1000000007 % MOD = 0
    assert(pancakeValueSum(2, {1000000007LL, 1}) == 1*1 + 1*2 = 3); // first contributes 0, second min=1 for both subarrays lengths 1 and 2 -> 1+2=3
    // Stress small with brute force verification for random small arrays could be included, but here we do a few.
    return 0;
}
