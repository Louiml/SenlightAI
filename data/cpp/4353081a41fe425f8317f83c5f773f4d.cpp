// Write a C++ function `int nextArithmeticTerm(int first, int second)` that takes two integers representing the first two terms of an arithmetic sequence and returns the third term. The function must work for both increasing and decreasing sequences, including sequences with a zero common difference, and it must handle negative integers and extreme `int` values correctly (i.e., the result must fit within the `int` range). The function should be self-contained, use `const` where appropriate, and not print anything to the console.
#include <cassert>

int nextArithmeticTerm(int first, int second); // Declaration for testing.

int main() {
    // Basic increasing sequence.
    assert(nextArithmeticTerm(2, 5) == 8);      // 2,5,8
    // Decreasing sequence.
    assert(nextArithmeticTerm(10, 7) == 4);     // 10,7,4
    // Zero common difference.
    assert(nextArithmeticTerm(-3, -3) == -3);   // -3,-3,-3
    // Negative values.
    assert(nextArithmeticTerm(-8, -2) == 4);    // -8,-2,4
    // Large positive values (still within int range).
    assert(nextArithmeticTerm(1000000, 2000000) == 3000000);
    // Large negative values.
    assert(nextArithmeticTerm(-5000, -8000) == -11000);
    // Single-step increment.
    assert(nextArithmeticTerm(0, 1) == 2);      // 0,1,2
    // Single-step decrement.
    assert(nextArithmeticTerm(0, -1) == -2);    // 0,-1,-2
    // Mixed signs.
    assert(nextArithmeticTerm(-5, 5) == 15);    // -5,5,15
    return 0;
}
#include <cstddef> // Not needed, but included for completeness if using size_t.

// Given the first two terms of an arithmetic sequence, return the third term.
// The common difference is (second - first), and the third term is second + difference.
int nextArithmeticTerm(const int first, const int second) {
    return second + (second - first);
}
// The core insight is that in an arithmetic sequence, the difference between consecutive terms is constant. Given the first term `a` and the second term `b`, the common difference is `d = b - a`. The third term is then obtained by adding that difference to the second term: `third = b + d = b + (b - a)`. This directly matches the provided snippet’s logic. Edge cases include `a == b` (zero difference, so the third term equals both) and negative differences (which naturally produce decreasing values). A potential overflow issue arises if `b - a` or `b + (b - a)` exceeds the `int` range; since the operation is performed with `int`, the problem statement implicitly assumes the inputs and result are within the valid `int` range. The algorithm uses only constant-time arithmetic operations, so time complexity is O(1) and space complexity is O(1) (no extra data structures). No special handling is required for negative numbers or zero because integer arithmetic handles them naturally.
