/*
Write a C++ function named `maxIceCreamsPurchasable` that takes a `std::vector<int>` of ice cream costs (where each element represents the price of a single ice cream bar in coins) and an integer `coins` representing the total coins available. The function should return the maximum number of ice cream bars that can be purchased, assuming the shopkeeper will sell bars in any order and you may buy any subset (greedy approach is optimal here: always buy the cheapest available first). The input vector may be empty (then the answer is 0), may contain duplicate costs, and the coins parameter can be any non-negative integer. The function should not modify the input vector.
*/
#include <vector>
#include <algorithm>

// Returns the maximum number of ice cream bars purchasable with the given coins.
// The vector costs is NOT modified; we sort a local copy.
int maxIceCreamsPurchasable(const std::vector<int>& costs, int coins) {
    // Make a local copy to sort without altering the input.
    std::vector<int> sorted_costs = costs;
    std::sort(sorted_costs.begin(), sorted_costs.end());
    
    int count = 0;
    int remaining = coins;
    
    for (int cost : sorted_costs) {
        if (cost > remaining) {
            break; // Cannot afford this or any later (more expensive) bar.
        }
        remaining -= cost;
        ++count;
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxIceCreamsPurchasable({1, 3, 2, 4, 1}, 7) == 4); // buy 1,1,2,3 = 7
    assert(maxIceCreamsPurchasable({10, 6, 8, 7, 7, 8}, 5) == 0); // cheapest is 6 > 5
    assert(maxIceCreamsPurchasable({1, 6, 3, 1, 2, 5}, 20) == 6); // all affordable, sum=18

    // Edge cases
    assert(maxIceCreamsPurchasable({}, 10) == 0); // empty vector
    assert(maxIceCreamsPurchasable({5}, 5) == 1); // exactly one bar
    assert(maxIceCreamsPurchasable({5}, 4) == 0); // not enough
    assert(maxIceCreamsPurchasable({2, 2, 2, 2}, 7) == 3); // duplicates, 2+2+2=6, next 2 >1 remaining

    // Input vector not modified (const correctness check)
    std::vector<int> costs = {3, 1, 2};
    int result = maxIceCreamsPurchasable(costs, 6);
    assert(result == 3);
    assert(costs[0] == 3 && costs[1] == 1 && costs[2] == 2); // original order preserved
}
// The optimal strategy is to sort the costs in ascending order and then iterate through the sorted list, subtracting each cost from the remaining coins as long as we have enough. If at any point the current cost exceeds the remaining coins, we stop because all subsequent costs are equal or larger. This greedy choice is optimal because buying the cheapest bars first maximizes the count for any fixed budget. Edge cases: empty vector returns 0; if the cheapest bar costs more than total coins, the loop breaks immediately and returns 0; duplicate costs are handled naturally since they are just equal values in the sorted list. Time complexity is \(O(n \log n)\) due to sorting, where \(n\) is the number of costs. Space complexity is \(O(1)\) extra space (ignoring the sort's internal stack), as we sort in-place (using a local copy to preserve the input). The loop itself is \(O(n)\).
