Write a C++ function named `findFirstMultipleInRange` that takes three integer parameters `A`, `B`, and `C` (where `A <= B`, `C >= 1`, and all values are within the range 1 to 1000 inclusive) and returns the smallest integer `x` such that `A <= x <= B` and `x` is divisible by `C`. If no such integer exists in the range, return `-1`. The function must be iterative, not using recursion, and must not use the `break` or `continue` statements or any other control flow beyond a simple `for` loop and an `if` check. The function should be `const`-correct and self-contained with only standard library includes needed.
// The solution approach is to iterate through all integers from `A` to `B` in increasing order and check for the first one that is divisible by `C`. Since we need the smallest such number, a simple linear scan from `A` upward is optimal in terms of clarity and correctness. The edge case where `C` is larger than `B` means no multiple can exist within the range, so we return `-1`. Another edge case is when `A` itself is divisible by `C`, then the answer is `A`. The loop can be optimized by starting at the first multiple of `C` that is >= `A`, which is computed as `((A + C - 1) / C) * C`. If that value is <= `B`, return it; otherwise return `-1`. This reduces time complexity from `O(B-A)` to `O(1)` arithmetic operations. The time complexity is O(1) and space complexity is O(1). This approach handles all valid inputs, including when `A` equals `B` and when `C` is 1 (in which case every integer is a multiple, so the answer is `A`).
#include <cstdint>

// Finds the smallest integer x such that A <= x <= B and x is divisible by C.
// Returns -1 if no such integer exists.
int findFirstMultipleInRange(int A, int B, int C) {
    // Compute the smallest multiple of C that is >= A.
    // Using integer arithmetic to avoid overflow.
    int firstMultiple = ((A + C - 1) / C) * C;
    
    // If this multiple is within the allowed range, return it; otherwise -1.
    if (firstMultiple <= B) {
        return firstMultiple;
    }
    return -1;
}
#include <cassert>

int findFirstMultipleInRange(int, int, int); // forward declaration

int main() {
    // Basic cases
    assert(findFirstMultipleInRange(1, 10, 3) == 3);
    assert(findFirstMultipleInRange(5, 20, 7) == 7);
    assert(findFirstMultipleInRange(10, 10, 2) == 10);
    
    // Edge: A itself is divisible
    assert(findFirstMultipleInRange(4, 8, 4) == 4);
    
    // Edge: C larger than range
    assert(findFirstMultipleInRange(1, 5, 10) == -1);
    
    // Edge: C = 1, always first number
    assert(findFirstMultipleInRange(3, 7, 1) == 3);
    
    // Edge: range start not divisible, but next is
    assert(findFirstMultipleInRange(5, 5, 2) == -1); // no multiple in single-element range
    assert(findFirstMultipleInRange(6, 6, 2) == 6);
    
    // Edge: larger numbers, within limits
    assert(findFirstMultipleInRange(999, 1000, 1000) == 1000);
    assert(findFirstMultipleInRange(998, 999, 1000) == -1);
    
    return 0;
}
