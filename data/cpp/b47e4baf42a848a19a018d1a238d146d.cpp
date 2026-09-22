// Write a C++ function `countExceedances` that takes four integers `a`, `b`, `c`, and `d` as input and returns an integer representing how many of the last three numbers (`b`, `c`, `d`) are strictly greater than the first number `a`. For example, if `a=5`, `b=6`, `c=5`, `d=7`, the result is `2` because `b` and `d` are greater than `a`, while `c` is not. The function should handle all integer values, including negative numbers and zero, and must not modify the inputs.
// The solution is straightforward: compare each of `b`, `c`, and `d` individually against `a` using the `>` operator. Since a boolean expression in C++ converts to `1` when true and `0` when false, you can sum the three boolean results directly to get the count of values strictly greater than `a`. This avoids any branching logic and is both concise and efficient. Edge cases include equal values (which should not count, as the comparison is strictly greater) and negative numbers (which work naturally with the comparison operator). The algorithm runs in constant time `O(1)` and uses constant auxiliary space `O(1)`, as it only performs three fixed comparisons and a summation.
#include <cstdint>

// Count how many of b, c, d are strictly greater than a.
int countExceedances(int a, int b, int c, int d) {
    return (b > a) + (c > a) + (d > a);
}
#include <cassert>

int main() {
    // Basic case
    assert(countExceedances(5, 6, 5, 7) == 2);
    // All greater
    assert(countExceedances(0, 1, 2, 3) == 3);
    // None greater
    assert(countExceedances(10, 9, 10, 8) == 0);
    // All equal
    assert(countExceedances(-3, -3, -3, -3) == 0);
    // Negative numbers
    assert(countExceedances(-5, -4, -6, -3) == 2);
    // Mixed signs
    assert(countExceedances(-1, 0, -2, 1) == 2);
    // Large values
    assert(countExceedances(1000000, 999999, 1000001, 1000000) == 1);
    // Only first greater, rest equal
    assert(countExceedances(7, 8, 7, 7) == 1);
    // Only last greater
    assert(countExceedances(4, 4, 4, 5) == 1);
    // All different, middle only greater
    assert(countExceedances(3, 2, 4, 2) == 1);
    return 0;
}
