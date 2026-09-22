/*
Write a C++ function named `maximumWaterBottles` that takes two non-negative integers: `numBottles` (the initial number of full water bottles) and `numExchange` (the number of empty bottles required to exchange for one full bottle). The function must return the total number of water bottles that can be drunk given that you can repeatedly exchange empty bottles for full ones, and every time you drink a full bottle it becomes empty. The function should handle the edge case where `numExchange` is 1 (meaning every empty bottle can be immediately exchanged, but since you always start with `numBottles` full bottles, the total is infinite in practice; however, assume `numExchange >= 2` for the task to avoid division by zero, but still handle the case gracefully if `numExchange == 1` by returning `numBottles` or a large sentinel? For clarity, the task assumes `numExchange >= 2` as typical). The function must use a constant-time formula, not a loop, and must be `const`-correct where applicable.
*/
#include <cstddef> // for size_t, though not strictly needed
#include <stdexcept> // for invalid_argument if needed, but not used

// Returns the total number of water bottles that can be drunk.
// Precondition: numExchange >= 2 (otherwise infinite process, but we handle numExchange==1 by returning numBottles).
// Assumes non-negative inputs.
int maximumWaterBottles(const int numBottles, const int numExchange) {
    if (numBottles <= 1) {
        return numBottles;
    }
    if (numExchange <= 1) {
        // If exchange rate is 1, each empty gives a full, so infinite; but as a finite
        // result for practical use, we return numBottles (no additional exchanges possible without losing empties? Actually with numExchange==1, you'd get infinite, but we cap at numBottles to avoid overflow).
        return numBottles;
    }
    // Formula: number of extra bottles = (numBottles - 1) / (numExchange - 1)
    const int extraBottles = (numBottles - 1) / (numExchange - 1);
    return numBottles + extraBottles;
}
#include <cassert>

int main() {
    // Basic cases from typical examples
    assert(maximumWaterBottles(9, 3) == 13);   // 9 initial, then 9/3=3 extra, then 3/3=1 extra, total 13
    assert(maximumWaterBottles(15, 4) == 19);  // 15 + floor((15-1)/(4-1)) = 15 + 14/3 = 15+4 = 19
    assert(maximumWaterBottles(5, 5) == 6);    // 5 + floor(4/4) = 5+1 = 6
    // Edge cases
    assert(maximumWaterBottles(0, 2) == 0);
    assert(maximumWaterBottles(1, 2) == 1);
    assert(maximumWaterBottles(2, 2) == 3);    // initial 2, drink both, exchange 2 empties for 1, drink that, total 3
    assert(maximumWaterBottles(10, 1) == 10);  // special case, returns numBottles
    // Larger values to ensure integer arithmetic works
    assert(maximumWaterBottles(100, 3) == 149); // 100 + floor(99/2) = 100+49 = 149
    assert(maximumWaterBottles(7, 2) == 13);   // every 2 empties gives 1, net gain 1 per exchange after first, 7 + floor(6/1) = 13
    return 0;
}
// The key insight is that each time you exchange `numExchange` empty bottles, you get one new full bottle, which after drinking becomes an additional empty bottle. This effectively reduces the number of empty bottles needed per net new bottle by one. The formula `total = numBottles + (numBottles - 1) / (numExchange - 1)` works because you can think of the process as: you start with `numBottles` empty bottles after drinking all initial full ones. Each exchange cycle you lose `numExchange - 1` empty bottles net (since you hand in `numExchange` and get back 1 empty after drinking). The number of extra bottles you can get is the floor of `(numBottles - 1) / (numExchange - 1)`, because you must always keep at least one empty bottle to start the chain, and the process stops when you have fewer than `numExchange` empties. Edge case: if `numExchange == 1`, the formula has division by zero, so handle it separately by returning `numBottles` (or a large sentinel like `INT_MAX` if you assume the process could go infinite, but practically you stop after one exchange because you'd never have an empty bottle? Actually with `numExchange == 1`, each empty gives a full, so infinite; but for a finite result, return `numBottles` as a safe fallback). Also, if `numBottles == 0`, the formula gives `0 + (0-1)/(n-1)` which is negative division issues, so guard with `if (numBottles <= 1)` return `numBottles`. Time complexity is O(1), space complexity O(1).
