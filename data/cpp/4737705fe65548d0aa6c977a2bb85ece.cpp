// Write a C++ function named `maxValueWithWeightLimit` that takes four parameters: a positive integer `capacity` representing the maximum total weight the knapsack can hold, an array of non-negative integers `weights` representing the weight of each item, an array of non-negative integers `values` representing the value of each item, and an integer `n` indicating the number of items. The function must return the maximum total value that can be achieved by selecting a subset of the items such that the sum of their weights does not exceed `capacity`. Each item can be used at most once. The function must be implemented using a recursive approach without memoization (i.e., plain recursion), and it must correctly handle cases where `n` is zero, `capacity` is zero, or an item’s weight exceeds the remaining capacity.

// The problem is the classic 0/1 knapsack solved via recursive exhaustive search. The recurrence is: for each item index `i` (from `n-1` down to 0), we either skip the item (resulting in the same capacity with `i-1` items) or, if the item’s weight does not exceed the current capacity, we include it and add its value to the best result achievable with remaining capacity `capacity - weights[i]` and the remaining `i-1` items. The base case occurs when there are no items left or the capacity is zero, in which case the value is 0. The choice that yields the larger value is returned. Edge cases include an empty item set (returns 0), zero capacity (returns 0), and items heavier than the current capacity (forced to be skipped). The recursion explores all \(2^n\) subsets, so time complexity is \(O(2^n)\) and space complexity is \(O(n)\) due to the recursion stack. This exponential time is acceptable for the small `n` in the original snippet but must be noted.

#include <algorithm> // for std::max

// Recursively compute the maximum value obtainable with a capacity-constrained knapsack.
// Each item can be used at most once. Items are considered from the last index down to 0.
// capacity: maximum total weight allowed (>0 in normal use, but 0 is handled)
// weights: array of item weights (non-negative)
// values: array of item values (non-negative)
// n: number of items (>=0)
int maxValueWithWeightLimit(int capacity, const int weights[], const int values[], int n) {
    // Base case: no items left or no remaining capacity.
    if (n == 0 || capacity == 0) {
        return 0;
    }
    // If the current item's weight exceeds the remaining capacity, skip it.
    if (weights[n - 1] > capacity) {
        return maxValueWithWeightLimit(capacity, weights, values, n - 1);
    }
    // Otherwise, choose the better of skipping or including the current item.
    const int skipValue = maxValueWithWeightLimit(capacity, weights, values, n - 1);
    const int includeValue = values[n - 1] + maxValueWithWeightLimit(capacity - weights[n - 1], weights, values, n - 1);
    return std::max(skipValue, includeValue);
}

#include <cassert>

int maxValueWithWeightLimit(int, const int[], const int[], int); // declaration

int main() {
    // Original example from the snippet: capacity 100, values {100,200,300}, weights {50,20,30}
    int weights1[] = {50, 20, 30};
    int values1[] = {100, 200, 300};
    assert(maxValueWithWeightLimit(100, weights1, values1, 3) == 600); // All items fit: 50+20+30=100, total value 600

    // Empty item set returns 0
    assert(maxValueWithWeightLimit(100, weights1, values1, 0) == 0);

    // Zero capacity returns 0
    assert(maxValueWithWeightLimit(0, weights1, values1, 3) == 0);

    // Single item that fits
    int w2[] = {5};
    int v2[] = {42};
    assert(maxValueWithWeightLimit(5, w2, v2, 1) == 42);

    // Single item that does not fit
    assert(maxValueWithWeightLimit(4, w2, v2, 1) == 0);

    // Multiple items where only some fit due to capacity constraints
    int w3[] = {10, 20, 30};
    int v3[] = {60, 100, 120};
    assert(maxValueWithWeightLimit(50, w3, v3, 3) == 220); // Choose items 2 and 3 (20+30=50) for value 220

    // All items heavier than capacity -> returns 0
    int w4[] = {10, 20};
    int v4[] = {5, 7};
    assert(maxValueWithWeightLimit(5, w4, v4, 2) == 0);

    // Capacity exactly equals the sum of all weights -> take all
    assert(maxValueWithWeightLimit(30, w4, v4, 2) == 12);

    // Large capacity but identical weights and values
    int w5[] = {3, 3, 3};
    int v5[] = {5, 5, 5};
    assert(maxValueWithWeightLimit(9, w5, v5, 3) == 15);

    return 0;
}
