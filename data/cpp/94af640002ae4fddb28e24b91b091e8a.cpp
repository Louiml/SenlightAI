Given an array of `n` integers, write a C++ function that returns the minimum possible sum after modifying the array so that it becomes non-decreasing. In one operation, you may decrease any element to any smaller integer value (including negative values). The goal is to make the final array non-decreasing (i.e., `final[i] <= final[i+1]` for all `i`), and the cost is the sum of absolute differences between the original and final values. The length `n` satisfies `1 <= n <= 5000`, and each `a[i]` satisfies `-10^9 <= a[i] <= 10^9`. Return the minimal total cost as a `long long`.

The problem is equivalent to finding a non-decreasing sequence `b[0] <= b[1] <= ... <= b[n-1]` that minimizes `sum(|a[i] - b[i]|)`. This is the classic "isotonic regression" problem under L1 distance, and an optimal solution can be found using dynamic programming. Since the optimal `b[i]` values can be restricted to the set of distinct original array values (proved by a standard exchange argument), we first sort and deduplicate the array to get candidate values `vals`. Let `m` be the number of distinct values. We define `f[i][j]` as the minimal cost to make the first `i+1` elements non-decreasing with the constraint that `b[i] = vals[j]`. The transition is: `f[i][j] = |a[i] - vals[j]| + min_{t <= j} f[i-1][t]`. We maintain a running minimum over `j` by updating from left to right, using only two rows (current and previous) to reduce memory. The initial row for `i=0` is simply `|a[0] - vals[j]|` with a prefix minimum. The answer is `min_j f[n-1][j]`. This works because if we ever need an intermediate value not in `vals`, shifting it to the nearest `vals` value preserves non-decreasing order and does not increase cost. Time complexity is `O(n * m)` where `m <= n`, so `O(n^2)` worst case (5000^2 = 25e6, fine). Space complexity is `O(m)` for the two rows, plus `O(n)` for sorting/storing values.

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>

// Returns the minimum total cost to make the array non-decreasing
// by replacing each element with any value, paying absolute difference.
long long minCostNonDecreasing(std::vector<long long> a) {
    int n = static_cast<int>(a.size());
    if (n == 0) return 0;

    std::vector<long long> vals = a;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    int m = static_cast<int>(vals.size());

    // prev[j] = minimal cost for first i elements ending with value vals[j]
    std::vector<long long> prev(m), curr(m);

    // Initialize for i = 0
    prev[0] = std::llabs(a[0] - vals[0]);
    for (int j = 1; j < m; ++j) {
        prev[j] = std::min(std::llabs(a[0] - vals[j]), prev[j-1]);
    }

    // Process remaining elements
    for (int i = 1; i < n; ++i) {
        // j = 0: previous must also end at vals[0]
        curr[0] = prev[0] + std::llabs(a[i] - vals[0]);
        // j > 0: use running minimum of prev[0..j]
        for (int j = 1; j < m; ++j) {
            curr[j] = std::min(curr[j-1], prev[j] + std::llabs(a[i] - vals[j]));
        }
        std::swap(prev, curr);
    }

    // Answer is min over all possible final values
    return *std::min_element(prev.begin(), prev.end());
}

#include <cassert>
#include <vector>

// The solution function is declared above (in the same translation unit in real usage).
// Here we call it directly.

int main() {
    // Already non-decreasing: no cost
    assert(minCostNonDecreasing(std::vector<long long>{1,2,3,4}) == 0);

    // Decreasing array: minimal cost is to make all equal to median (2)
    // Options: [2,2,2] cost = |3-2|+|2-2|+|1-2| = 2
    assert(minCostNonDecreasing(std::vector<long long>{3,2,1}) == 2);

    // Single element: cost 0
    assert(minCostNonDecreasing(std::vector<long long>{-5}) == 0);

    // All equal: cost 0
    assert(minCostNonDecreasing(std::vector<long long>{7,7,7}) == 0);

    // Mixed with negatives: e.g., [-2, -5, 1] -> make non-decreasing
    // Best: [-2, -2, 1] cost = 0 + 3 + 0 = 3
    // Or [-2,-1,1] cost=0+4+0=4; [-5,-5,1] cost=3+0+0=3; so 3
    assert(minCostNonDecreasing(std::vector<long long>{-2, -5, 1}) == 3);

    // Large values
    assert(minCostNonDecreasing(std::vector<long long>{1000000000LL, -1000000000LL}) == 2000000000LL);

    // Reverse sorted with duplicates
    // [5,4,4,3] -> best make all 4? cost =1+0+0+1=2; or all 3? cost=2+1+1+0=4; so 2
    assert(minCostNonDecreasing(std::vector<long long>{5,4,4,3}) == 2);

    // Already non-decreasing with duplicates
    assert(minCostNonDecreasing(std::vector<long long>{1,1,2,2,3}) == 0);
}
