Write a C++ function `int boundedKnapsack(int n, int capacity, const std::vector<int>& weights, const std::vector<int>& values, const std::vector<int>& counts)` that solves the bounded knapsack problem: given `n` item types, each with weight `w[i]`, value `v[i]`, and at most `c[i]` copies, choose a multiset of items (respecting per-type limits) with total weight ≤ `capacity` to maximize total value. The total weight capacity `capacity` and all `w[i]`, `v[i]`, `c[i]` are positive integers, and `n` is positive. The function must handle arbitrarily large capacity (up to 10^5) and counts (up to 10^3), with total items across all types possibly large (e.g., sum of counts up to 10^7). The function must run efficiently using a monotonic queue (sliding window maximum) over each remainder class modulo `w[i]` to achieve O(n * capacity) time and O(capacity) extra space (beyond input vectors). Return the maximum achievable value.
#include <cassert>
#include <vector>

// The solution function is declared above (include it or link appropriately).
// Here we provide a main function with assertions.

int main() {
    // Basic example: one item type, weight=2, value=3, count=2, capacity=5
    // Possible: take 2 copies (weight 4, value 6) or 1 copy (value 3) or 0. Max = 6.
    assert(boundedKnapsack(1, 5, {2}, {3}, {2}) == 6);

    // Two types, classic bounded knapsack
    // Type0: w=3, v=4, c=2 ; Type1: w=2, v=3, c=3 ; capacity=7
    // Options: best is 2*type0 (6 weight, value 8) + none type1 → 8
    // Or 1*type0 (3,4) + 2*type1 (4,6)=7 weight, value 10 → better.
    // Or 0 type0 + 3*type1 (6,9) → 9. So max=10.
    assert(boundedKnapsack(2, 7, {3, 2}, {4, 3}, {2, 3}) == 10);

    // Count limit acts as 0/1: count=1 each
    // capacity=4, weights {1,2,3}, values {2,3,4}, counts {1,1,1}
    // Can take weight 1+3=4 value 6, or 2+1=3 value5, or 3 value4, etc. Max=6.
    assert(boundedKnapsack(3, 4, {1, 2, 3}, {2, 3, 4}, {1, 1, 1}) == 6);

    // High count effectively unbounded: one type w=2, v=5, c=1000, capacity=10
    // Can take 5 copies weight 10 value 25. Max=25.
    assert(boundedKnapsack(1, 10, {2}, {5}, {1000}) == 25);

    // Edge: capacity smaller than any item weight → 0
    assert(boundedKnapsack(2, 3, {5, 6}, {10, 20}, {1, 1}) == 0);

    // Edge: capacity exactly matches a single item weight
    // one type w=4, v=7, c=1, capacity=4 → 7
    assert(boundedKnapsack(1, 4, {4}, {7}, {1}) == 7);

    // Multiple types with large counts but small capacity
    // capacity=10, type0 w=6 v=8 c=2, type1 w=4 v=5 c=3
    // Options: 1*type0 (6,8) + 1*type1 (4,5)=10 weight value13; or 2*type0 (12>10) no; or 2*type1 (8,10)+? none; max=13
    assert(boundedKnapsack(2, 10, {6, 4}, {8, 5}, {2, 3}) == 13);

    // Edge: zero copies allowed (count=0) – though problem says positive counts, just to be safe
    // one type w=1 v=10 c=0 capacity=5 → 0
    assert(boundedKnapsack(1, 5, {1}, {10}, {0}) == 0);

    // Large capacity test (no heavy computation, just correctness for small n)
    // capacity=100, one type w=3 v=2 c=10 → can take 10 copies weight30 value20, or say 33 copies weight99 value66 (since c=10 max 10 copies) so 20
    assert(boundedKnapsack(1, 100, {3}, {2}, {10}) == 20);

    // Mixed counts and weights, verify monotonic queue handles window sliding
    // capacity=10, type0 w=2 v=3 c=2, type1 w=3 v=4 c=1
    // Options: 2*type0 (4,6)+1*type1 (3,4)=7 value10; or 1*type0 (2,3)+? no, etc. Max=10.
    assert(boundedKnapsack(2, 10, {2, 3}, {3, 4}, {2, 1}) == 10);

    return 0;
}
#include <vector>
#include <deque>
#include <algorithm>

// Solves the bounded knapsack problem: maximize total value subject to weight limit
// and per-item-type count limits. Returns the maximum achievable total value.
int boundedKnapsack(int n, int capacity,
                    const std::vector<int>& weights,
                    const std::vector<int>& values,
                    const std::vector<int>& counts) {
    std::vector<int> dp(capacity + 1, 0);
    std::vector<int> prev(capacity + 1, 0);
    std::deque<int> dq; // stores indices (capacity values) for current remainder class

    for (int i = 0; i < n; ++i) {
        int w = weights[i];
        int v = values[i];
        int c = counts[i];
        if (w > capacity) continue; // item too heavy, cannot use even one copy

        prev = dp; // copy current best values (O(capacity))

        // Process each remainder class modulo w
        for (int r = 0; r < w; ++r) {
            dq.clear();
            // Iterate over k = r, r+w, r+2w, ...
            for (int k = r; k <= capacity; k += w) {
                int step = (k - r) / w; // number of steps from r to k

                // Remove from front if too many copies would be used (step difference > c)
                if (!dq.empty() && step - (dq.front() - r) / w > c) {
                    dq.pop_front();
                }

                // Compute adjusted value for current k: prev[k] - step*v
                int current_adjusted = prev[k] - step * v;

                // Maintain decreasing order in deque by adjusted value
                while (!dq.empty()) {
                    int back_idx = dq.back();
                    int back_step = (back_idx - r) / w;
                    int back_adjusted = prev[back_idx] - back_step * v;
                    if (back_adjusted <= current_adjusted) {
                        dq.pop_back();
                    } else {
                        break;
                    }
                }

                // Now push current k (will be considered as predecessor for later steps)
                dq.push_back(k);

                // Best value using capacity k: take the front element (best adjusted)
                if (!dq.empty()) {
                    int best_idx = dq.front();
                    int best_step = (best_idx - r) / w;
                    int candidate = prev[best_idx] + (step - best_step) * v;
                    dp[k] = std::max(dp[k], candidate);
                }
            }
        }
    }
    return dp[capacity];
}
// This problem is a classic bounded knapsack. The straightforward approach—treating each copy as an individual item—would be O(capacity * totalCopies) which is too slow (e.g., 10^5 * 10^7). Instead, we use a monotonic queue optimization over the DP array.
//
// We maintain `dp[j]` = max value using capacity `j` up to the current item type. For each item type `i`, we copy `dp` to `prev`. For each remainder class `r` in `[0, w[i]-1]`, we iterate `k = r, r+w[i], r+2*w[i], ...` up to `capacity`. For each such `k`, we consider taking `t` copies (0 ≤ t ≤ c[i]) where `t*w[i] ≤ k`. The recurrence is:
// `dp[k] = max( prev[k - t*w[i]] + t*v[i] )` for t in [0, c[i]] with `k - t*w[i] ≥ 0`.
//
// Rewrite by letting `a = (k - r)/w[i]` (the number of steps), and define `value_index(a, k) = prev[k] - a*v[i]`. Then `dp[k] = max over t of (prev[k - t*w[i]] - ((k - t*w[i] - r)/w[i])*v[i]) + a*v[i]`. So we can maintain a deque of candidate indices (in terms of the step count) with decreasing `prev[position] - stepCount*v[i]`. We slide the window: for each `k`, before computing, we pop from front those that are more than `c[i]` steps behind the current step. We also maintain the deque so that new candidate `prev[k]` is inserted after popping from back while its adjusted value is ≥ the back’s adjusted value. The front of deque gives the best predecessor, so `dp[k] = prev[front] + (k - front)/w[i] * v[i]` (if deque not empty). Edge cases: when `c[i]` is huge, effectively unbounded, the window never pops from front (except when exceeding capacity), so it behaves as unbounded knapsack. When `c[i]=1` it reduces to 0/1 knapsack with the same O(n*capacity). Initialization: all `dp` entries are 0 (since we can choose nothing). Final answer is `dp[capacity]`.
//
// Time complexity: O(n * capacity) because each item type processes each capacity exactly once. Space: O(capacity) for the DP arrays plus O(capacity) worst-case for the deque, so O(capacity) total extra.
