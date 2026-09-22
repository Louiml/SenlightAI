/*
Write a C++ function `kthRemovedBySieve(int n, int k)` that simulates a modified Sieve of Eratosthenes process. Starting with all integers from 2 to `n` unmarked, repeatedly find the smallest unmarked number `i`, then mark every multiple of `i` (including `i` itself) that is still unmarked, counting each newly marked number. The function must stop and return the `k`-th number that gets marked during this process (1-indexed). If the process completes (all numbers from 2 to `n` are marked) before reaching the `k`-th mark, return `-1`. For example, with `n=10` and `k=6`, the marking order is: 2 (index1), 4 (2), 6 (3), 8 (4), 10 (5), 3 (6) → return 3. The function should handle edge cases like `k=1` (returns 2) and large `k` beyond total marks (returns -1).
*/
#include <vector>

// Simulates a modified Sieve of Eratosthenes that returns the k-th marked number.
// Returns -1 if fewer than k numbers get marked.
int kthRemovedBySieve(int n, int k) {
    if (n < 2 || k < 1) return -1;
    if (k > n - 1) return -1; // only numbers 2..n can be marked, total n-1

    std::vector<bool> marked(n + 1, false);
    int markCount = 0;

    for (int i = 2; i <= n; ++i) {
        if (marked[i]) continue; // already marked by a smaller prime
        for (int j = i; j <= n; j += i) {
            if (!marked[j]) {
                marked[j] = true;
                ++markCount;
                if (markCount == k) {
                    return j;
                }
            }
        }
    }
    return -1; // should never reach here if k <= n-1
}
#include <cassert>

int main() {
    // Basic cases
    assert(kthRemovedBySieve(10, 1) == 2);
    assert(kthRemovedBySieve(10, 2) == 4);
    assert(kthRemovedBySieve(10, 3) == 6);
    assert(kthRemovedBySieve(10, 4) == 8);
    assert(kthRemovedBySieve(10, 5) == 10);
    assert(kthRemovedBySieve(10, 6) == 3);
    assert(kthRemovedBySieve(10, 7) == 9);
    assert(kthRemovedBySieve(10, 8) == 5);
    assert(kthRemovedBySieve(10, 9) == 7);

    // Edge cases
    assert(kthRemovedBySieve(2, 1) == 2);
    assert(kthRemovedBySieve(2, 2) == -1);
    assert(kthRemovedBySieve(1, 1) == -1);
    assert(kthRemovedBySieve(5, 5) == -1); // only 4 numbers to mark
    assert(kthRemovedBySieve(5, 4) == 5);
}
// The algorithm mimics a sieve but with explicit marking order. Maintain a boolean vector `marked` of size `n+1`, initially all `false`. Iterate `i` from 2 upward; if `i` is already marked, skip it (since it was marked by a smaller prime, and its multiples have already been considered). If `i` is unmarked, then for every multiple `j = i, 2i, 3i, ...` up to `n`, check if `j` is unmarked. If so, mark it and increment a counter; if the counter equals `k`, return `j`. Continue until `i > n` or we find the answer. Edge cases: `n` may be 1 (no numbers to mark), `k` may be larger than the total number of marks (total marks = number of integers 2..n = n-1), so return -1. Also, if `k` is 1, the first marked is 2 (when i=2). Time complexity: The outer loop runs up to `n`, and inner loops mark multiples; each number is marked exactly once, but the inner loops still run for every unmarked `i`, leading to O(n log log n) marking steps, but because we also check unmarked status every time, it's O(n log log n) plus the overhead of scanning multiples of primes. More precisely, for each prime `p`, we iterate n/p multiples, so total work is O(n log log n). Space complexity is O(n) for the boolean vector.
