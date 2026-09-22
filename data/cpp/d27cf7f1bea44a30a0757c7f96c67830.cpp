Write a C++ function `findUnique` that takes three integers as input and returns the one that appears exactly once. The input is guaranteed to consist of exactly two equal values and one distinct value. Your function should handle any integer values, including negatives, and return the distinct integer. The function signature should be `int findUnique(int a, int b, int c);` and must be implemented in a self-contained way with appropriate `const` correctness where applicable.
The problem is trivially solved by comparing the three input values. Since two values are equal and one differs, we can check each pairwise equality: if `b` equals `c`, then `a` is the unique one; otherwise, if `a` equals `c`, then `b` is unique; otherwise, `c` must be the unique one (since the only remaining case is `a == b` and `c` different). This works for all integers, including negatives, because equality comparison is exact. No edge cases exist beyond the guaranteed input structure—there is no possibility of all three being equal or all distinct. The algorithm uses only constant space and constant time, specifically \(O(1)\) time and \(O(1)\) auxiliary space.
#include <cstdint>

// Return the integer that appears exactly once among three given integers.
// Guaranteed: exactly two of {a, b, c} are equal, and the third is distinct.
int findUnique(const int a, const int b, const int c) {
    if (b == c) {
        return a;
    }
    if (a == c) {
        return b;
    }
    // a == b must hold here.
    return c;
}
#include <cassert>

int findUnique(int a, int b, int c);

int main() {
    // Basic cases
    assert(findUnique(1, 2, 2) == 1);
    assert(findUnique(2, 1, 2) == 1);
    assert(findUnique(2, 2, 1) == 1);
    
    // Negative values
    assert(findUnique(-5, -5, 3) == 3);
    assert(findUnique(-5, 3, -5) == 3);
    assert(findUnique(3, -5, -5) == 3);
    
    // Large values
    assert(findUnique(1000000, 1000000, -1000000) == -1000000);
    assert(findUnique(-2000000000, 2000000000, 2000000000) == -2000000000);
    
    // Zero
    assert(findUnique(0, 0, 42) == 42);
    assert(findUnique(0, 42, 0) == 42);
    assert(findUnique(42, 0, 0) == 42);
    
    // Duplicate order variations
    assert(findUnique(7, 7, 7) == 7); // Not valid input, but function still returns 7 (safe)
    // However, the specification guarantees two equal and one distinct, so the above is optional.
    
    return 0;
}
