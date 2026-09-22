/*
Write a C++ function that computes the square of a non-negative integer using only bitwise operations and recursion, without using the multiplication operator or any built-in power functions. The function must handle zero and positive integers, and it must process the integer in a divide-and-conquer manner based on its binary representation. The solution must be efficient for large integers up to at least \(10^6\), and it should return the exact mathematical square as an `int`. Input validation is not required because the function will only be called with non-negative values by the test harness.
*/

#include <cstdint>

// Compute the square of a non-negative integer using bitwise recursion.
// Algorithm: (2k)^2 = 4k^2, (2k+1)^2 = 4k^2 + 4k + 1.
int squareRecursive(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    int half = n >> 1;  // n / 2
    int halfSquare = squareRecursive(half);

    if (n & 1) {
        // Odd: (2k+1)^2 = 4k^2 + 4k + 1
        return (halfSquare << 2) + (half << 2) + 1;
    } else {
        // Even: (2k)^2 = 4k^2
        return halfSquare << 2;
    }
}

#include <cassert>

int main() {
    // Basic cases
    assert(squareRecursive(0) == 0);
    assert(squareRecursive(1) == 1);
    assert(squareRecursive(2) == 4);
    assert(squareRecursive(3) == 9);
    assert(squareRecursive(4) == 16);
    assert(squareRecursive(5) == 25);

    // Larger values
    assert(squareRecursive(10) == 100);
    assert(squareRecursive(15) == 225);
    assert(squareRecursive(100) == 10000);
    assert(squareRecursive(123) == 15129);
    assert(squareRecursive(1000) == 1000000);

    // Edge near max for int (46340^2 fits in int)
    assert(squareRecursive(46340) == 2147395600);
    assert(squareRecursive(46341) == 2147488281);

    // Random spot checks
    assert(squareRecursive(7) == 49);
    assert(squareRecursive(12) == 144);
    assert(squareRecursive(99) == 9801);
}

// The algorithm exploits the identity \((2k)^2 = 4k^2\) and \((2k+1)^2 = 4k^2 + 4k + 1\). Recursively, we compute the square of the integer divided by 2 (obtained via a right shift), then multiply the result by 4 (via a left shift by 2) for even numbers, and additionally add \(4k+1\) for odd numbers. The recursion terminates when `n` is 0 or 1. Edge cases include `n = 0` returning 0, `n = 1` returning 1, and ensuring no overflow occurs for the given input range (though for values up to \(10^6\), squares fit safely in `int`). Since the recursion reduces `n` by half each step, the depth is \(O(\log n)\). Each recursive call does constant work, so time complexity is \(O(\log n)\) and space complexity is \(O(\log n)\) due to the call stack.
