Write a standalone C++ function named `minimumSplitDifference` that takes a non-empty vector of non-negative integers (weights) and returns the smallest possible absolute difference between the total weight of a prefix (the first k elements where k can be 0 to n) and the remaining suffix (the rest of the array). Equivalently, for each possible split point k, compute `abs(total_sum - 2 * prefix_sum(k))` and return the minimum over all k from 0 to n, where prefix_sum(0)=0 and prefix_sum(n)=total_sum. Example: for weights `[3,1,4]`, possible prefix sums are 0,3,4,8, giving differences |8-0|=8, |8-6|=2, |8-8|=0, |8-16|=8 → minimum is 0. The function must have signature `int minimumSplitDifference(const std::vector<int>& weights);`.
// The core observation is that for a split after k elements, the prefix sum is `pref[k]`, and the suffix sum is `total - pref[k]`. The absolute difference between suffix and prefix is `|(total - pref[k]) - pref[k]| = |total - 2*pref[k]|`. Therefore we only need to compute all prefix sums and evaluate that expression for each k from 0 to n (where prefix sums array length is n+1 with first element 0). We can build the prefix sums in O(n) time and evaluate each candidate in O(1), yielding total O(n) time and O(n) auxiliary space for the prefix array (though we could also do it in O(1) extra space by maintaining the running prefix inside a single loop, but the prefix array is clearer). Edge cases: n=1 (e.g., weights=[5]) → k=0 gives |5-0|=5, k=1 gives |5-10|=5 → answer 5; empty input is not allowed per non-empty constraint, but if it were, we’d return 0. The algorithm handles duplicate weights and zeros naturally. Complexity: O(n) time, O(n) space (or O(1) if we use a rolling variable, but O(n) is fine and the task expects a simple prefix sum approach).
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>

// Returns the minimum absolute difference between a prefix sum and the remaining suffix sum.
int minimumSplitDifference(const std::vector<int>& weights) {
    if (weights.empty()) return 0; // safety, though task says non-empty

    // Build prefix sums: pref[0] = 0, pref[i] = sum of first i weights.
    const int n = static_cast<int>(weights.size());
    std::vector<int> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + weights[i];
    }

    const int total = pref[n];
    int best = INT_MAX;

    // Consider every split point k from 0 to n.
    for (int k = 0; k <= n; ++k) {
        const int diff = std::abs(total - 2 * pref[k]);
        best = std::min(best, diff);
    }

    return best;
}
#include <cassert>
#include <vector>
#include "solution.h" // assuming the above is in solution.h (or copy the function here)

int main() {
    // Basic cases
    assert(minimumSplitDifference({3, 1, 4}) == 0);
    assert(minimumSplitDifference({1, 2, 3}) == 0); // total=6, prefix sums 0,1,3,6 → |6-6|=0
    assert(minimumSplitDifference({1, 1}) == 0); // splits give |2-0|=2, |2-2|=0, |2-4|=2
    assert(minimumSplitDifference({5}) == 5); // single element
    assert(minimumSplitDifference({10, 10, 10}) == 10); // total=30, best diff 10

    // Larger random check with known answer
    std::vector<int> w1 = {7, 2, 5, 10, 8};
    // prefix sums: 0,7,9,14,24,32 → diffs: 32,18,14,4,16,32 → min is 4
    assert(minimumSplitDifference(w1) == 4);

    // All zeros
    assert(minimumSplitDifference({0, 0, 0}) == 0);

    // Mixed zeros and positives
    assert(minimumSplitDifference({0, 5, 0}) == 0); // total=5, split after 1 (prefix=5) gives 0

    // Large numbers
    assert(minimumSplitDifference({1000, 2000, 3000}) == 0); // total=6000, pref 0,1000,3000,6000 → |6000-6000|=0

    // Already split evenly
    std::vector<int> w2 = {2, 2, 2, 2}; // total=8, best diff 0
    assert(minimumSplitDifference(w2) == 0);

    return 0;
}
