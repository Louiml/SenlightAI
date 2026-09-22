// Write a C++ function named `canSplitWatermelonEvenly` that takes an integer `w` representing the weight of a watermelon and returns a `bool` indicating whether the watermelon can be split into two even (positive) parts. The weight must be greater than 2 (since each part must have at least 1 kg), and the total weight must be even. Your function should handle edge cases such as `w = 2` (too small to split into two positive evens) and `w = 0` or negative values (return `false`). The function must be pure (no I/O) and use `const` correctness where applicable.
The problem reduces to checking two conditions: the weight must be even and greater than 2. If `w` is odd, it cannot be split into two even numbers because the sum of two evens is always even. If `w` is even but equals 2, the only split is 1+1, but 1 is odd (and not even), so it fails. For any even `w >= 4`, a valid split exists (e.g., 2 and `w-2`). Edge cases: zero and negative weights return `false` since weights must be positive. The solution is a simple O(1) check with constant space complexity. No loops or data structures are required.
#include <cstdbool>

// Return true if a watermelon of given weight can be split into two even positive parts.
bool canSplitWatermelonEvenly(const int weight) {
    // Must be at least 4 (2+2) and even
    return (weight >= 4) && (weight % 2 == 0);
}
int main() {
    assert(canSplitWatermelonEvenly(4) == true);
    assert(canSplitWatermelonEvenly(8) == true);
    assert(canSplitWatermelonEvenly(10) == true);
    assert(canSplitWatermelonEvenly(2) == false);
    assert(canSplitWatermelonEvenly(3) == false);
    assert(canSplitWatermelonEvenly(7) == false);
    assert(canSplitWatermelonEvenly(0) == false);
    assert(canSplitWatermelonEvenly(-5) == false);
}
