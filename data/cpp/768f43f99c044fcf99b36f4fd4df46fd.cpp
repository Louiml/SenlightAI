// Given two positive integers A and B, you must determine the minimum number of equal-sized squares of side length B that can be used to completely cover a square of side length A, without overlap or extending beyond the boundary. The squares may be placed in a regular grid, and you must count the total number of such B×B squares required. Write a C++ function that takes A and B as parameters and returns this minimum number. You may assume A is a multiple of B (i.e., A = B * k for some positive integer k). The function should be efficient for large values (up to 10^9) and return the result as a 64-bit integer.

// The problem reduces to finding how many B×B squares tile an A×A square exactly. Since A is a multiple of B, we can fit A/B squares along each side, giving (A/B) * (A/B) total squares. The provided snippet uses a loop that subtracts B from A repeatedly, counting squares column by column (or row by row), which is inefficient for large values. The optimal solution is direct: compute k = A / B (integer division) and return k * k. Edge cases: if B > A, then A is not a multiple (unless A=0, but positive integers given), but per constraints A is a multiple of B; still, integer division handles it gracefully by giving 0, but we could assert A % B == 0 for safety. Complexity: O(1) time and O(1) space.

#include <cstdint>

// Returns the minimum number of B×B squares needed to tile an A×A square.
// Assumes A is a positive multiple of B.
std::int64_t countSquares(std::int64_t A, std::int64_t B) {
    // Number of squares per side
    auto per_side = A / B;
    // Total squares = per_side^2
    return per_side * per_side;
}

#include <cassert>
#include <cstdint>

// Declaration of the function under test (provided elsewhere)
std::int64_t countSquares(std::int64_t A, std::int64_t B);

int main() {
    // Basic cases
    assert(countSquares(1, 1) == 1);        // 1x1 square
    assert(countSquares(4, 2) == 4);        // 2x2 tiles in a 4x4 square
    assert(countSquares(9, 3) == 9);        // 3x3 tiles
    assert(countSquares(10, 5) == 4);       // 2x2 tiles
    assert(countSquares(100, 10) == 100);   // 10x10 tiles
    // Large value
    assert(countSquares(1000000000, 1) == 1000000000000000000LL); // 10^9 squared
    // Multiple of larger size
    assert(countSquares(100, 25) == 16);    // 4x4 tiles
    assert(countSquares(12, 4) == 9);       // 3x3 tiles
    // A equals B
    assert(countSquares(7, 7) == 1);
    // A is 10 times B
    assert(countSquares(20, 2) == 100);     // 10x10 tiles
    return 0;
}
