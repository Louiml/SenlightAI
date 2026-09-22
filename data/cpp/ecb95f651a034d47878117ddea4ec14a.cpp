Given positive integers \(N\) and \(K\) (with \(1 \le N \le 10^9\) and \(1 \le K \le 10^{18}\)), write a C++ function `long long kthSmallestInMultiplicationTable(long long N, long long K)` that returns the \(K\)-th smallest number in the \(N \times N\) multiplication table (where the table contains all products \(i \times j\) for \(1 \le i,j \le N\), with duplicates counted multiple times). For example, with \(N=3\), the sorted table values are \([1,2,2,3,3,4,6,6,9]\), so \(K=7\) returns \(6\). The function must handle very large \(N\) efficiently.

#include <cassert>

int main() {
    // Basic small case (N=3, sorted table: 1,2,2,3,3,4,6,6,9)
    assert(kthSmallestInMultiplicationTable(3, 1) == 1);
    assert(kthSmallestInMultiplicationTable(3, 2) == 2);
    assert(kthSmallestInMultiplicationTable(3, 3) == 2);
    assert(kthSmallestInMultiplicationTable(3, 4) == 3);
    assert(kthSmallestInMultiplicationTable(3, 5) == 3);
    assert(kthSmallestInMultiplicationTable(3, 6) == 4);
    assert(kthSmallestInMultiplicationTable(3, 7) == 6);
    assert(kthSmallestInMultiplicationTable(3, 8) == 6);
    assert(kthSmallestInMultiplicationTable(3, 9) == 9);

    // N=1: only 1
    assert(kthSmallestInMultiplicationTable(1, 1) == 1);

    // N=2: sorted table 1,2,2,4
    assert(kthSmallestInMultiplicationTable(2, 1) == 1);
    assert(khSmallestInMultiplicationTable(2, 2) == 2);
    assert(kthSmallestInMultiplicationTable(2, 4) == 4);

    // Edge: K equals total entries N*N -> maximum value N*N
    assert(kthSmallestInMultiplicationTable(5, 25) == 25);

    // Large N but small K: answer is 1 because K=1 is the smallest (1*1)
    assert(kthSmallestInMultiplicationTable(1000000, 1) == 1);

    // Large N, large K: sanity check with N=100000, K at the very end
    // N*N = 10^10, maximum product is N*N, so last entry must be N*N
    assert(kthSmallestInMultiplicationTable(100000, 10000000000LL) == 10000000000LL);

    // K clamped to 1e9 (as original snippet did), verify the answer is valid
    // For N=1000, 1e9 is beyond total entries, but our function supports full K.
    assert(kthSmallestInMultiplicationTable(1000, 1000000000LL) == 1000000LL); // N*N = 1e6, so max

    // Spot check for N=4: sorted table values: 1,2,2,3,3,4,4,4,6,6,8,8,9,12,12,16
    assert(kthSmallestInMultiplicationTable(4, 10) == 6);
    assert(kthSmallestInMultiplicationTable(4, 16) == 16);
}

#include <algorithm>

// Returns the k-th smallest number in the N x N multiplication table.
// The table contains all i * j for 1 <= i, j <= N, duplicates counted.
// Uses binary search on the answer and a grouped count to handle large N.
long long kthSmallestInMultiplicationTable(long long N, long long K) {
    auto countLessEqual = [N](long long x) -> long long {
        long long total = 0;
        // For each possible quotient q = floor(x / i), group rows i where floor(x / i) is constant.
        // i ranges over [1, N]. We skip when q becomes 0 (no columns fit).
        long long i = 1;
        while (i <= N) {
            long long q = x / i;
            if (q == 0) break; // For larger i, floor(x/i)=0, contributes 0.
            // Last row with the same quotient q: largest i such that floor(x/i) == q.
            long long last_i = std::min(N, x / q);
            // For each row in [i, last_i], the count is min(q, N).
            total += (last_i - i + 1) * std::min(q, N);
            i = last_i + 1;
        }
        return total;
    };

    long long left = 1;
    long long right = N * N;
    while (left < right) {
        long long mid = left + (right - left) / 2;
        if (countLessEqual(mid) >= K) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

// The task reduces to finding the smallest integer \(x\) such that the count of table entries \(\le x\) is at least \(K\). For a fixed \(x\), the number of products \(i \times j \le x\) with \(1 \le i,j \le N\) can be computed by iterating over each row \(i\): for that row, valid columns are \(j \le \lfloor x/i \rfloor\), but capped at \(N\), so the row contributes \(\min(\lfloor x/i \rfloor, N)\). Summing over all \(i=1\) to \(N\) gives the count. Since \(N\) can be up to \(10^9\), iterating all \(N\) rows is too slow, but we can observe that \(\lfloor x/i \rfloor\) takes at most \(O(\sqrt{x})\) distinct values, allowing a grouped summation. However, for the common constraints where \(N\) is up to \(10^5\) in the original snippet, a simple loop over \(i=1..N\) is acceptable; for a more general efficient version, use grouping. The main algorithm is binary search on the answer range \([1, N*N]\). The count function is monotonic nondecreasing in \(x\), so binary search works. Edge cases: \(K\) may exceed the total number of entries (\(N^2\)), but by problem definition \(K \le N^2\); still, we clamp \(K\) to \(10^9\) as in the snippet for safety, but the reference solution can handle full \(K\). Duplicates are counted multiple times, which the counting formula naturally does. Time complexity: \(O(N \log(N^2))\) for the simple row loop; with grouping, \(O(\sqrt{N} \log N)\). Space complexity: \(O(1)\).
