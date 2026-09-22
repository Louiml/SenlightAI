You are at a warehouse distributing items across `n` store locations. You are given a vector of positive integers `quantities`, where `quantities[i]` is the total number of units of product type `i` that need to be distributed. Each store can receive at most `k` units of any single product type, and all units of a product type must be distributed (you can split a product type across multiple stores, but each store gets no more than `k` units of that type). Write a C++ function `int minimizedMaximum(int n, const vector<int>& quantities)` that returns the smallest possible value of `k` such that every product type can be fully distributed to at most `n` stores while respecting the per-store per-product limit. If `n` is large enough to hold all individual units (i.e., `k=1`), return `1`. You may assume `n >= 1` and `quantities` is non-empty with all values ≥ 1.

The problem is a classic binary search on the answer. For a candidate value `k`, we check if it is feasible: for each product type `i`, the number of stores needed is `ceil(quantities[i] / k)`, which can be computed as `(quantities[i] + k - 1) / k` (avoiding floating point). Sum these over all product types; if the total is ≤ `n`, then `k` is feasible. The search space for `k` is from `1` to the maximum quantity in the input (since if `k` equals the maximum quantity, each product type fits in one store). We perform binary search: if `mid` is feasible, we try a smaller value (`high = mid - 1`) and record `mid` as a candidate answer; otherwise, we need a larger `k` (`low = mid + 1`). Edge cases: if `n` is exactly the number of product types, then `k = max(quantities)` is feasible (each type to one store), but we might find a smaller `k` if some products are small enough to share stores. If all quantities are `1`, the answer is `1` regardless of `n` because each product type needs exactly one store (or could be combined if `n` is larger, but `1` is the minimum). Complexity: each feasibility check is `O(m)` where `m = quantities.size()`, and binary search runs `O(log(max(quantities)))` iterations, so total time `O(m log max(quantities))` and `O(1)` auxiliary space.

#include <vector>
#include <algorithm>

// Returns the smallest maximum per-store limit k such that all product types
// can be distributed across at most n stores.
int minimizedMaximum(int n, const std::vector<int>& quantities) {
    int low = 1;
    int high = *std::max_element(quantities.begin(), quantities.end());
    int answer = high;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        long long stores_needed = 0;
        for (int q : quantities) {
            // Ceiling division: ceil(q / mid)
            stores_needed += (q + mid - 1) / mid;
            if (stores_needed > n) {
                break; // early exit to save time
            }
        }
        if (stores_needed <= n) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// Assume minimizedMaximum is defined in the solution section above.

int main() {
    // Basic case
    assert(minimizedMaximum(6, {11, 6}) == 3);
    // Explanation: with k=3, stores needed = ceil(11/3)+ceil(6/3)=4+2=6

    // Case where each product fits one store
    assert(minimizedMaximum(3, {5, 7, 9}) == 9);

    // Case where n is very large, single unit products
    assert(minimizedMaximum(10, {1, 1, 1}) == 1);

    // Single product type, n=1
    assert(minimizedMaximum(1, {5}) == 5);

    // Single product type, n=5
    assert(minimizedMaximum(5, {5}) == 1);

    // Duplicate quantities
    assert(minimizedMaximum(4, {4, 4, 4}) == 4);

    // Mixed large and small values
    assert(minimizedMaximum(7, {10, 2, 3, 1}) == 3);
    // With k=3: ceil(10/3)=4, ceil(2/3)=1, ceil(3/3)=1, ceil(1/3)=1 -> total=7

    // Edge: n=1 with multiple products (must be max value)
    assert(minimizedMaximum(1, {3, 8, 2}) == 8);

    // Edge: all products larger than n but can be split
    assert(minimizedMaximum(2, {100, 100}) == 100);

    // Larger n allows lower k
    assert(minimizedMaximum(10, {10, 10, 10}) == 4);
    // With k=4: ceil(10/4)=3 each, total=9 ≤10

    return 0;
}
