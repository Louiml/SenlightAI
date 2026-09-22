Write a C++ function that takes a single positive integer `n` representing the number of energy bars a person buys, and returns the net cost in rupees. Each energy bar costs 800 rupees, but for every 15 bars purchased, the store gives a discount of 200 rupees on the total bill (the discount applies once per full group of 15 bars, regardless of the remainder). The function should compute and return the total amount the person actually pays. For example, if `n = 30`, the person buys 30 bars: 30 × 800 = 24000 rupees, and since 30 / 15 = 2 full groups, the discount is 2 × 200 = 400, so the net cost is 23600. The function must handle any non-negative integer input; if `n` is less than 15, no discount is given.

// The solution is straightforward arithmetic. The total cost before discount is `n * 800`. The number of full groups of 15 is computed as `n / 15` using integer division (which automatically floors for non-negative integers). The total discount is `(n / 15) * 200`. The net cost is the total cost minus the discount. Edge cases include `n = 0` (returns 0), `n` less than 15 (discount = 0), and large `n` where the multiplication may overflow if using 32-bit integers—so we use `long long` to safely handle typical test inputs. The time complexity is O(1) and space complexity is O(1).

#include <cstdint>

// Compute the net cost of buying n energy bars with the given pricing scheme.
long long netCost(int n) {
    // Use long long to avoid overflow for large inputs.
    long long bars = n;
    long long fullGroups = bars / 15;
    long long totalCost = bars * 800;
    long long discount = fullGroups * 200;
    return totalCost - discount;
}

#include <cassert>

int main() {
    assert(netCost(0) == 0);
    assert(netCost(1) == 800);
    assert(netCost(14) == 11200);
    assert(netCost(15) == 12000 - 200); // Should be 11800
    assert(netCost(16) == 12800 - 200); // Should be 12600
    assert(netCost(30) == 24000 - 400); // Should be 23600
    assert(netCost(45) == 36000 - 600); // Should be 35400
    assert(netCost(100) == 80000 - 1200); // Should be 78800
    assert(netCost(150) == 120000 - 2000); // Should be 118000
    return 0;
}
