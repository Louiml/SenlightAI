// Write a C++ function `countValidTriples(int n, int k)` that, given two positive integers `n` and `k`, returns the number of ordered triples `(i, j, m)` with `1 ≤ i ≤ n`, `1 ≤ j ≤ n`, and `1 ≤ m ≤ n`, such that `i + j + m == k`. The function must compute this directly without generating all triples, using a loop over two variables and a condition for the third. Assume `1 ≤ n ≤ 10^6` and `1 ≤ k ≤ 3*n`. Return the count as an `int`. The function should be efficient enough to handle the upper bound of `n` within a few seconds.
// The problem asks for the count of all ordered triples whose sum equals `k`. Since each element is bounded between 1 and `n`, we can iterate over the first two values `i` and `j`, and check whether `m = k - i - j` lies within the valid range `[1, n]`. If it does, that triple is valid. This avoids a third nested loop, reducing time from O(n³) to O(n²). For each `i` from 1 to n, and for each `j` from 1 to n, we compute `m = k - i - j`. The condition is `m >= 1 && m <= n`. The count is incremented for each such pair. Edge cases: if `k` is too small (less than 3) or too large (greater than 3n), the count might be zero, but the loop still runs correctly due to the condition. Also, note that the order of `(i,j,m)` matters, so we count every valid ordered pair `(i,j)` that leads to a valid `m`. Time complexity is O(n²), space complexity is O(1). For `n = 10^6`, O(n²) is too slow, so a more optimized approach is possible: For a fixed `i`, the valid `j` range can be computed as `max(1, k - i - n) ≤ j ≤ min(n, k - i - 1)`, which gives the count of valid `j` in O(1) per `i`, making the total O(n). That is the optimal solution. We'll implement that: for each `i`, compute `low = max(1, k - i - n)` and `high = min(n, k - i - 1)`, then add `max(0, high - low + 1)`. This works because for a given `i`, `j` must satisfy `1 ≤ j ≤ n` and `1 ≤ k - i - j ≤ n` → `k - i - n ≤ j ≤ k - i - 1`. Complexity: O(n) time, O(1) space.
#include <algorithm>

// Count the number of ordered triples (i, j, m) with 1 <= i,j,m <= n such that i+j+m == k.
// Uses an O(n) sweep over i, calculating the range of valid j in O(1).
int countValidTriples(int n, int k) {
    if (k < 3 || k > 3 * n) {
        return 0;
    }

    int total = 0;
    for (int i = 1; i <= n; ++i) {
        // For fixed i, j must satisfy: 1 <= j <= n and 1 <= k - i - j <= n
        // which is equivalent to: k - i - n <= j <= k - i - 1
        int low = std::max(1, k - i - n);
        int high = std::min(n, k - i - 1);
        if (high >= low) {
            total += (high - low + 1);
        }
    }
    return total;
}
#include <cassert>

int main() {
    // Brute-force reference for small n
    auto brute = [](int n, int k) {
        int cnt = 0;
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                for (int m = 1; m <= n; ++m)
                    if (i + j + m == k) ++cnt;
        return cnt;
    };

    // Edge cases
    assert(countValidTriples(1, 3) == 1); // (1,1,1)
    assert(countValidTriples(1, 2) == 0);
    assert(countValidTriples(1, 4) == 0);
    assert(countValidTriples(3, 9) == 1); // (3,3,3)
    assert(countValidTriples(3, 10) == 0);

    // Compare with brute force for small n and a range of k
    for (int n = 1; n <= 5; ++n) {
        for (int k = 1; k <= 15; ++k) {
            assert(countValidTriples(n, k) == brute(n, k));
        }
    }

    // Larger random checks (n=100, some k)
    for (int n = 100, k = 150; k <= 250; ++k) {
        assert(countValidTriples(n, k) == brute(n, k));
    }

    // Performance sanity: n=1000000, k=1500000 should run fast and return a known count
    // (exact value is computed by the formula: for each i, count valid j)
    int large_n = 1000000;
    int large_k = 1500000;
    int result = countValidTriples(large_n, large_k);
    assert(result > 0); // just a sanity check, not exact
}
