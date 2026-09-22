Write a C++ function named `sumOfOddNumbersInRange` that takes two integers, `a` and `b`, and returns the sum of all odd integers in the inclusive range from `min(a, b)` to `max(a, b)`. The function must handle cases where the inputs are equal, negative, or where the smaller value is greater than the larger value (i.e., the range is defined by the absolute endpoints). The function should be `const`-correct, use only standard library facilities, and return the result as an `int`. If the range contains no odd numbers (e.g., both endpoints are even and adjacent), the function should return `0`. The function must not print anything or read from standard input; it only computes and returns the sum.
The core algorithm is straightforward: first, determine the lower and upper bounds of the range by taking the minimum and maximum of the two input integers (this handles when `a > b`). Then, iterate from the lower bound to the upper bound, inclusive, and accumulate the value of each integer that is odd (i.e., `i % 2 != 0`). Since the range is inclusive, the loop must use `<=` for the upper bound. Edge cases include: (1) when both inputs are equal — the loop runs once, and if that number is odd, it returns that number; otherwise 0; (2) when the range contains negative numbers — the modulo operator works correctly for negatives in C++ (e.g., `-3 % 2 == -1`, which is not equal to 0, so it correctly identifies odd numbers); (3) when the range is empty? — this never happens because the lower bound is always ≤ upper bound after using `min`/`max`. Time complexity is `O(n)` where `n = |b - a| + 1`, and space complexity is `O(1)` because we only use a few local variables. An alternative more efficient formula using arithmetic series exists, but the iterative approach is simpler and sufficient for the task.
#include <algorithm>  // for std::min, std::max

// Returns the sum of all odd integers in the inclusive range [a, b] (order independent).
// Handles negative numbers, equal endpoints, and returns 0 if no odd numbers exist.
int sumOfOddNumbersInRange(int a, int b) {
    const int lower = std::min(a, b);
    const int upper = std::max(a, b);

    int sum = 0;
    for (int i = lower; i <= upper; ++i) {
        if (i % 2 != 0) {  // C++ modulo works with negatives; odd numbers have remainder ≠ 0
            sum += i;
        }
    }
    return sum;
}
#include <cassert>

// Assume the solution function is defined above.
int main() {
    // Basic positive range
    assert(sumOfOddNumbersInRange(1, 5) == 9);       // 1+3+5 = 9
    // Reverse order (a > b)
    assert(sumOfOddNumbersInRange(5, 1) == 9);
    // Equal endpoints, odd
    assert(sumOfOddNumbersInRange(7, 7) == 7);
    // Equal endpoints, even
    assert(sumOfOddNumbersInRange(4, 4) == 0);
    // Negative and positive range
    assert(sumOfOddNumbersInRange(-3, 3) == 0);      // -3 + -1 + 1 + 3 = 0
    // All negative range
    assert(sumOfOddNumbersInRange(-5, -1) == -9);    // -5 + -3 + -1 = -9
    // Range with no odd numbers (two even adjacent)
    assert(sumOfOddNumbersInRange(2, 4) == 3);       // only 3 is odd, sum=3
    // Large range (1 to 100) — sum of first 50 odd numbers = 50^2 = 2500
    assert(sumOfOddNumbersInRange(1, 100) == 2500);
    // Range with zero
    assert(sumOfOddNumbersInRange(-2, 2) == 0);      // -1 + 1 = 0
    // Duplicate even endpoints with one odd in between
    assert(sumOfOddNumbersInRange(10, 12) == 11);
    return 0;
}
