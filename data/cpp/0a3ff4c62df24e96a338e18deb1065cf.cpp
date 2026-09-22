/*
Write a C++ function `int minimumCoinsToBuyFruits(std::vector<int> prices)` that, given a non-empty vector of positive integers representing the prices of fruits arranged in a sequence, returns the minimum total cost required to buy all fruits under the following rule: when you buy the fruit at index `i` (0-based), you receive the next `i+1` fruits for free (i.e., indices `i+1` through `2*i+1`, clamped to the array bounds), but you must pay for at least one fruit in every "block" starting from index 0. The cost is computed by repeatedly merging: starting from the last fruit that would be bought first (index `(n-1)/2`), for each step, you must pay the minimum price among the free fruits you would skip, and add that minimum to the current fruit's price, then move one index left. Continue until index 0 holds the total cost. The function must not modify the input vector (work on a copy internally). The input may contain duplicate prices, and the vector size can be as small as 1.
*/

#include <vector>
#include <climits>
#include <algorithm>

// Compute the minimum coins needed to buy all fruits according to the merging rule.
// The input vector is not modified; we work on a copy.
int minimumCoinsToBuyFruits(const std::vector<int>& prices) {
    if (prices.empty()) return 0;

    std::vector<int> cost = prices;
    int n = static_cast<int>(cost.size());

    // Process from the last "buyer" back to the first
    for (int i = (n - 1) / 2; i > 0; --i) {
        int minPrice = INT_MAX;
        int minIdx = -1;

        // Find the minimum price in the free range for this index
        int end = std::min(2 * i + 1, n);
        for (int j = i; j < end; ++j) {
            if (cost[j] < minPrice) {
                minPrice = cost[j];
                minIdx = j;
            }
        }

        // Add the minimum free price to the current fruit's price
        cost[i - 1] += cost[minIdx];
    }

    return cost[0];
}

#include <cassert>
#include <vector>

int main() {
    // Example 1: {4,2,3} -> result 5
    assert(minimumCoinsToBuyFruits({4,2,3}) == 5);

    // Example 2: {2,11,2,2} -> result 4
    assert(minimumCoinsToBuyFruits({2,11,2,2}) == 4);

    // Example 3: {23,20,10,14,50,9,49,49} -> result 32
    assert(minimumCoinsToBuyFruits({23,20,10,14,50,9,49,49}) == 32);

    // Single fruit: cost is just its price
    assert(minimumCoinsToBuyFruits({7}) == 7);

    // Two fruits: pay min of both (since i=0 gives one free, but we must pay one)
    // Here prices = {5,3} -> process i=0? Actually loop starts at i=0? For n=2, (n-1)/2 = 0, loop does not run -> returns 5. But logically we should pay min(5,3)=3? Let's check: The rule: buy fruit 0 (price 5) get fruit 1 free => total 5. That's correct.
    assert(minimumCoinsToBuyFruits({5,3}) == 5);

    // Duplicate values: {3,3,3} -> process i=1, range [1,2] min=3, cost[0]=3+3=6
    assert(minimumCoinsToBuyFruits({3,3,3}) == 6);

    // Larger case: {1,2,3,4,5} 
    // Manual: copy={1,2,3,4,5}, n=5, i=2: range[2,4] min=3, cost[1]=2+3=5 -> {1,5,3,4,5}
    // i=1: range[1,2] min=3, cost[0]=1+3=4 -> result 4
    assert(minimumCoinsToBuyFruits({1,2,3,4,5}) == 4);

    // Input with large values: {10,20,30} -> i=1: range[1,2] min=20, cost[0]=10+20=30
    assert(minimumCoinsToBuyFruits({10,20,30}) == 30);

    // Verify original vector is not modified
    std::vector<int> original = {4,2,3};
    int result = minimumCoinsToBuyFruits(original);
    assert(result == 5);
    assert(original[0] == 4 && original[1] == 2 && original[2] == 3);

    return 0;
}

// The algorithm simulates a bottom-up merging process over the fruit prices. First, we copy the input vector to allow in-place modifications without affecting the caller. We then iterate from index `i = (n-1)/2` down to `1` (inclusive). For each `i`, we examine the range `[i, min(2*i+1, n-1)]` to find the index `minPriceIndex` of the smallest price within that range (ties broken arbitrarily, e.g., the first occurrence). We then update `prices[i-1] += prices[minPriceIndex]`; this represents the idea that to cover the fruits in that range, the cheapest among them must be paid for, and that cost is added to the predecessor fruit's price. The process continues until `i` reaches 1, after which `prices[0]` holds the total minimum coins. Edge cases: when `n=1`, the loop does not execute and the function returns `prices[0]` unchanged; duplicate values are handled naturally by selecting any minimum. Time complexity is \(O(n^2)\) in the worst case because for each `i` we scan up to `i+1` elements (the loop runs about `n/2` times). Space complexity is \(O(n)\) due to the copy of the input vector.
