// Write a C++ function `countPerfectSquaresInRange(int start, int end)` that takes two inclusive integer bounds and returns the number of integers within that range that are perfect squares (i.e., whose square root is an integer). For example, between 1 and 10, the perfect squares are 1, 4, and 9, so the count is 3. The range may include negative numbers, zero, and can be in either order (e.g., `start=10, end=1` should be treated as the range 1 to 10). The function must handle empty ranges gracefully (e.g., `start=5, end=5` includes only 5, which is not a square, so return 0). Do not use floating-point math for checking squares—use only integer operations.

The solution must first normalize the range so that `start` is the smaller bound and `end` is the larger bound, swapping if necessary. Then iterate through every integer from `start` to `end` (inclusive). For each integer `i`, we need to check if it's a perfect square. A robust integer-only approach is to use a loop that generates candidate squares: start with `root = 0` and compute `root*root`, incrementing `root` until the square exceeds `end`. For each candidate square, if it lies between `start` and `end`, increment the count. This avoids floating-point precision issues with `sqrt()` and handles negative numbers correctly (negative numbers are never perfect squares). The time complexity is \(O(\sqrt{\text{end}} + \text{range size})\) if we simply check each number, but the optimal method iterates only over roots, giving \(O(\sqrt{\text{end}})\) time and \(O(1)\) space. Edge cases: when `start > end` (swap), when the range contains zero (0 is a perfect square), and when the range is entirely negative (count = 0).

#include <algorithm>

// Counts how many integers in the inclusive range [start, end] are perfect squares.
// The range is normalized so that start <= end, and negative numbers never count.
// Uses only integer arithmetic to avoid floating-point inaccuracies.
int countPerfectSquaresInRange(int start, int end) {
    // Normalize range bounds.
    if (start > end) {
        std::swap(start, end);
    }
    // Negative numbers cannot be perfect squares, so start at 0 if needed.
    int lower = std::max(start, 0);
    if (lower > end) {
        return 0;
    }
    
    int count = 0;
    int root = 0;
    // Generate squares until they exceed the upper bound.
    while (root * root <= end) {
        long long square = static_cast<long long>(root) * root; // avoid overflow
        if (square >= lower) {
            ++count;
        }
        ++root;
    }
    return count;
}

#include <cassert>

int main() {
    // Basic ranges
    assert(countPerfectSquaresInRange(1, 10) == 3);   // 1,4,9
    assert(countPerfectSquaresInRange(0, 0) == 1);    // 0 is a square
    assert(countPerfectSquaresInRange(-5, 5) == 3);   // 0,1,4
    assert(countPerfectSquaresInRange(10, 1) == 3);   // swapped -> 1..10
    // Single non-square
    assert(countPerfectSquaresInRange(2, 2) == 0);
    // Entirely negative range
    assert(countPerfectSquaresInRange(-10, -1) == 0);
    // Larger range
    assert(countPerfectSquaresInRange(50, 100) == 3); // 64,81,100
    // Contains a large square
    assert(countPerfectSquaresInRange(9999, 10000) == 1); // 10000 = 100^2
    // Zero in middle
    assert(countPerfectSquaresInRange(-3, 3) == 2);   // 0,1
    // Start = end = perfect square
    assert(countPerfectSquaresInRange(16, 16) == 1);
    // Range with no squares
    assert(countPerfectSquaresInRange(17, 18) == 0);
}
