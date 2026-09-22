/*
Given a circular array of `n` positive integers (`1 <= n <= 10^5`, each `1 <= a[i] <= 10^9`), write a C++ function `int minimumRotationSteps(const vector<int>& a)` that returns the smallest non-negative integer `k` (`0 <= k < n`) such that every consecutive subarray of length `k+1` in the circular array has the same greatest common divisor (GCD). In other words, for all starting positions `i` from `0` to `n-1`, the GCD of the elements `a[i], a[(i+1)%n], ..., a[(i+k)%n]` must be identical. If `k=0` trivially works, return `0`. If only the full circle (length `n`) works, return `n-1`. The function must handle up to multiple test cases efficiently (total `n` across all cases ≤ 2·10^5). You are allowed to use a sparse table for range GCD queries and binary search. Note: the input is given as a vector `a` of length `n`, representing the circular array in order.
*/

#include <bits/stdc++.h>

// Returns the smallest k such that every circular subarray of length k+1 has the same GCD.
int minimumRotationSteps(const std::vector<int>& a) {
    int n = (int)a.size();
    if (n <= 1) return 0;

    // Build duplicated array: size 2n
    std::vector<int> arr(2 * n);
    for (int i = 0; i < n; ++i) {
        arr[i] = a[i];
        arr[i + n] = a[i];
    }

    const int LOG = 20; // since 2n <= 2e5, log2(2e5) ~ 18
    std::vector<std::vector<int>> st(2 * n, std::vector<int>(LOG));
    for (int i = 0; i < 2 * n; ++i) {
        st[i][0] = arr[i];
    }

    for (int j = 1; j < LOG; ++j) {
        int step = 1 << (j - 1);
        for (int i = 0; i + (1 << j) <= 2 * n; ++i) {
            st[i][j] = std::gcd(st[i][j - 1], st[i + step][j - 1]);
        }
    }

    // Precompute logs
    std::vector<int> logs(2 * n + 1, 0);
    for (int i = 2; i <= 2 * n; ++i) {
        logs[i] = logs[i / 2] + 1;
    }

    // Range GCD query on [L, R] inclusive
    auto getGCD = [&](int L, int R) -> int {
        int len = R - L + 1;
        int lg = logs[len];
        return std::gcd(st[L][lg], st[R - (1 << lg) + 1][lg]);
    };

    // Binary search the minimal k in [0, n-1]
    int low = 0, high = n - 1, answer = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2; // window length = mid+1
        int windowLen = mid + 1;

        // Check if all circular windows of this length have same GCD
        int firstGCD = getGCD(0, windowLen - 1);
        bool ok = true;
        for (int i = 1; i < n; ++i) {
            int curGCD = getGCD(i, i + windowLen - 1);
            if (curGCD != firstGCD) {
                ok = false;
                break;
            }
        }

        if (ok) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return answer;
}

#include <bits/stdc++.h>
#include <cassert>
// Include the solution function here (or link it)

int main() {
    // All equal elements -> k=0
    assert(minimumRotationSteps({5,5,5}) == 0);
    // Single element -> k=0
    assert(minimumRotationSteps({42}) == 0);
    // Example from typical problem: [1,2,3] circular, any k=0 gives GCDs 1,2,3 not equal; k=1 windows (1,2)=1, (2,3)=1, (3,1)=1 all equal -> answer 1
    assert(minimumRotationSteps({1,2,3}) == 1);
    // [2,4,8] -> k=0 gives 2,4,8 not equal; k=1 gives gcd(2,4)=2, gcd(4,8)=4 not equal; k=2 gives gcd(2,4,8)=2 all equal -> answer 2
    assert(minimumRotationSteps({2,4,8}) == 2);
    // [6,10,15] -> k=0: 6,10,15 differ; k=1: gcd(6,10)=2, gcd(10,15)=5 differ; k=2: gcd(all)=1 same -> answer 2
    assert(minimumRotationSteps({6,10,15}) == 2);
    // Already all same GCD for k=0? e.g., [2,2] -> k=0 works
    assert(minimumRotationSteps({2,2}) == 0);
    // Larger test: [1,1,2] circular -> k=0: 1,1,2 differ; k=1: gcd(1,1)=1, gcd(1,2)=1, gcd(2,1)=1 all equal -> answer 1
    assert(minimumRotationSteps({1,1,2}) == 1);
    // Test with coprime numbers [7,11] -> k=0: 7 and 11 differ; k=1: gcd(7,11)=1 same -> answer 1
    assert(minimumRotationSteps({7,11}) == 1);
    // Edge: n=2 with equal elements [3,3] -> k=0
    assert(minimumRotationSteps({3,3}) == 0);
    // n=5 all same GCD for full length only? [2,3,4,5,6] -> k=4 (full length) must work
    // Check: k=0 GCDs: 2,3,4,5,6 diff; k=1: gcd(2,3)=1, gcd(3,4)=1, gcd(4,5)=1, gcd(5,6)=1, gcd(6,2)=2 -> not all; ... eventually k=4 gives gcd all =1 -> answer 4
    assert(minimumRotationSteps({2,3,4,5,6}) == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The key observation is that as we increase the window length `k`, the GCD of any window can only stay the same or decrease (since adding more elements can only remove common factors). This monotonicity allows binary search on the answer `k`. For a fixed `k`, we need to check if all circular windows of length `w = k+1` have the same GCD. To query GCD of any circular subarray quickly, we duplicate the array to length `2n` and build a sparse table that supports `O(1)` range GCD queries. For a given window length `w`, for each start `i` from 0 to n-1, we query the GCD of the subarray `[i, i+w-1]` in the duplicated array. We then compare all these GCDs: if min equals max, then all are equal. Binary search finds the smallest `k` satisfying this. Edge cases: `n=1` → answer is `0` (only one window). If all elements are equal, the GCD is the element itself for any `k`, so answer is `0`. The binary search range is `[0, n-1]` because a window of length `n` always covers the whole array and returns the GCD of all elements, same for every start. Time complexity: sparse table build O(n log n), each check O(n) using O(1) queries, and binary search O(log n) steps → total O(n log n) per test case. Space O(n log n).
