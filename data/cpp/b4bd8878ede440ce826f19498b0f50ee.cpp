Write a C++ function named `canSplitWatermelon` that takes an integer `weight` representing the weight of a watermelon in kilograms and returns a boolean indicating whether the watermelon can be divided into two even, positive parts. The function should return `true` if and only if there exist two positive integers `a` and `b` such that `a + b = weight` and both `a` and `b` are even. For example, weight 8 can be split as 2+6 or 4+4, but weight 4 cannot be split because the only even split 2+2 has both parts positive but is allowed, wait—actually 4 can be split as 2+2 (both positive and even), so the correct rule is: a weight is splittable if it is even and greater than 2. Return `false` otherwise. The function must handle all integers from 1 to 10^9 inclusive.
The problem is a classic parity check. For a watermelon to be split into two positive even parts, the total weight must be even (since even + even = even), and the total must be at least 4 (because the smallest positive even number is 2, so the smallest possible sum of two positive even numbers is 2+2=4). Therefore, the condition is simply `weight % 2 == 0 && weight > 2`. Edge cases: weight 1, 2, 3 are not splittable; weight 4 is splittable (2+2). Large weights up to 10^9 fit in a standard 32-bit integer, but we use `int` safely. Time complexity is O(1) with constant space.
#include <cstdbool>

// Return true if a watermelon of given weight can be split into two positive even parts.
bool canSplitWatermelon(int weight) {
    // An even weight greater than 2 can always be split (e.g., 2 and weight-2).
    return (weight % 2 == 0) && (weight > 2);
}
#include <cassert>

int main() {
    // Basic valid splits
    assert(canSplitWatermelon(4) == true);   // 2+2
    assert(canSplitWatermelon(6) == true);   // 2+4
    assert(canSplitWatermelon(8) == true);   // 2+6
    assert(canSplitWatermelon(1000000000) == true); // large even

    // Invalid cases
    assert(canSplitWatermelon(1) == false);
    assert(canSplitWatermelon(2) == false);  // 1+1 not both even
    assert(canSplitWatermelon(3) == false);
    assert(canSplitWatermelon(5) == false);  // odd
    assert(canSplitWatermelon(0) == false);  // not positive parts
    assert(canSplitWatermelon(-4) == false); // negative weight
}
