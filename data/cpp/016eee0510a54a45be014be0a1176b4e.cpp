/*
Write a C++ function `calculateMomos` that takes a vector of shop prices (positive integers) and a single daily budget (positive integer), and returns a `std::pair<long long, long long>` representing (number of momos bought, money left). Shreya visits shops in order and buys exactly one momo from each shop until she cannot afford the next one. The function must handle up to \(10^5\) shops and queries efficiently, using a prefix-sum array for each query individually. The function should be reusable for multiple queries by being called separately for each budget. The vector of prices is not necessarily sorted, and the budget can be large (up to \(10^9\)). If she cannot buy any momo, return (0, budget). The function must be `const`-correct and avoid modifying the input.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Given a vector of shop prices and a daily budget, return the number of momos
// she can buy (buying one from each shop in order) and the money left over.
// The function uses a prefix-sum array and binary search for efficiency.
std::pair<long long, long long> calculateMomos(const std::vector<long long>& prices, long long budget) {
    // Build prefix sums: prefix[i] = sum of prices[0..i]
    std::vector<long long> prefix(prices.size());
    long long sum = 0;
    for (size_t i = 0; i < prices.size(); ++i) {
        sum += prices[i];
        prefix[i] = sum;
    }

    // Binary search for the largest index i such that prefix[i] <= budget
    // We look for upper_bound - 1, but since prefix is strictly increasing,
    // we can use std::upper_bound.
    auto it = std::upper_bound(prefix.begin(), prefix.end(), budget);
    if (it == prefix.begin()) {
        // Cannot afford even the first shop
        return {0, budget};
    }

    size_t count = it - prefix.begin(); // number of momos = index+1, since it points after the last valid
    long long moneySpent = prefix[count - 1];
    return {static_cast<long long>(count), budget - moneySpent};
}

#include <cassert>
#include <iostream>
int main() {
    // Sample from the problem
    std::vector<long long> prices = {2, 1, 6, 3};
    auto result = calculateMomos(prices, 11);
    assert(result.first == 3 && result.second == 2);

    // Single shop, can afford
    std::vector<long long> p1 = {5};
    result = calculateMomos(p1, 10);
    assert(result.first == 1 && result.second == 5);

    // Single shop, cannot afford
    result = calculateMomos(p1, 4);
    assert(result.first == 0 && result.second == 4);

    // Large budget buys all
    std::vector<long long> p2 = {3, 7, 2};
    result = calculateMomos(p2, 100);
    assert(result.first == 3 && result.second == 88);

    // Budget exactly equals a prefix sum
    std::vector<long long> p3 = {4, 2, 1};
    result = calculateMomos(p3, 7);
    assert(result.first == 3 && result.second == 0);

    // Non-sorted prices still work (order matters)
    std::vector<long long> p4 = {10, 1, 1, 1};
    result = calculateMomos(p4, 13);
    assert(result.first == 4 && result.second == 0);

    // Edge: zero budget
    result = calculateMomos(p4, 0);
    assert(result.first == 0 && result.second == 0);

    // Very large numbers
    std::vector<long long> p5 = {1000000000LL, 500000000LL};
    result = calculateMomos(p5, 1500000000LL);
    assert(result.first == 2 && result.second == 0);
    result = calculateMomos(p5, 1499999999LL);
    assert(result.first == 1 && result.second == 499999999LL);

    // Multiple calls with same prices
    result = calculateMomos(prices, 4); // 2+1=3, next 6 too much
    assert(result.first == 2 && result.second == 1);
    result = calculateMomos(prices, 2); // only first shop
    assert(result.first == 1 && result.second == 0);

    std::cout << "All tests passed!\n";
    return 0;
}

// The key insight is to precompute a prefix-sum array of the shop prices. For a given budget `X`, the number of momos she can buy is the largest index `i` such that `prefix[i] <= X`. Since the prefix sums are strictly increasing (all prices are positive), we can use binary search on the prefix-sum array. If no prefix sum is <= X, then she buys 0 momos and has `X` money left. Otherwise, after finding the largest valid index, the count of momos is that index+1, and money left is `X - prefix[index]`. We must handle edge cases: `n=1`, budget smaller than the first price, budget larger than the total sum. The binary search must avoid infinite loops by using a standard lower/upper bound approach. Time complexity: O(n) to build prefix sums once per call (or if we want to optimize for multiple queries, we would precompute outside but since the function is standalone per call, we build inside) and O(log n) for binary search. Space complexity: O(n) for the prefix array.
