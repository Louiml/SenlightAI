Given an array of positive integers and a target value, write a C++ function `int minMultipliers(const std::vector<int>& coins, int target)` that returns the minimum number of times a single element from the array must be multiplied to exactly equal the target. If no element divides the target evenly, return a large sentinel value (e.g., `INT_MAX`). The input array is non-empty, and both the array elements and the target are positive integers. The array may contain duplicate values, and the target may be smaller than some or all elements.

The solution iterates through each element in the array. For each element, we check if it divides the target exactly using the modulo operator (`target % value == 0`). If it does, the multiplier needed is `target / value`. We keep track of the minimum such multiplier across all elements. Edge cases include: if the target is 1 (but specified as positive, so ≥1), any element equal to 1 gives a multiplier of `target`; if no element divides the target, we return `INT_MAX` (or a large sentinel). The algorithm runs in **O(n)** time and uses **O(1)** extra space, where `n` is the size of the array. Note that we only consider multiplication of a single element, not combinations of different elements.

#include <vector>
#include <climits>
#include <algorithm>

// Returns the minimum number of times a single coin value must be multiplied
// to exactly equal target, or INT_MAX if no coin divides target evenly.
int minMultipliers(const std::vector<int>& coins, int target) {
    int best = INT_MAX;
    for (const int value : coins) {
        if (target % value == 0) {
            best = std::min(best, target / value);
        }
    }
    return best;
}

#include <cassert>
#include <vector>
#include <climits>

int main() {
    std::vector<int> coins1 = {2, 5, 10};
    assert(minMultipliers(coins1, 20) == 2);  // 10*2=20

    std::vector<int> coins2 = {4, 6, 9};
    assert(minMultipliers(coins2, 12) == 3);  // 4*3=12

    std::vector<int> coins3 = {3, 7};
    assert(minMultipliers(coins3, 10) == INT_MAX);  // no divisor

    std::vector<int> coins4 = {1, 5, 10};
    assert(minMultipliers(coins4, 7) == 7);  // 1*7=7

    std::vector<int> coins5 = {10, 5, 2};
    assert(minMultipliers(coins5, 10) == 1);  // 10*1 or 5*2 or 2*5, min is 1

    std::vector<int> coins6 = {6};
    assert(minMultipliers(coins6, 6) == 1);

    std::vector<int> coins7 = {6, 6, 6};
    assert(minMultipliers(coins7, 18) == 3);

    std::vector<int> coins8 = {100, 50, 25};
    assert(minMultipliers(coins8, 75) == INT_MAX);  // none divides 75

    std::vector<int> coins9 = {3, 3, 3};
    assert(minMultipliers(coins9, 3) == 1);
}
