Write a C++ function named `countDigits` that takes a non-negative integer `n` (where `n` can be up to 2,147,483,647) and returns the number of digits in its decimal representation. The function must handle `n = 0` correctly by returning `1` (since zero has one digit), and it should not modify the input argument. The function must be implemented using a loop that repeatedly divides the number by 10 (similar to the provided snippet) but with `n` passed as a `const` parameter (or copied internally) to preserve the original value. The function signature should be `int countDigits(int n);`.

The algorithm repeatedly divides the input number by 10, incrementing a counter each time, until the number becomes zero. However, the edge case `n = 0` must be handled separately because the loop condition `while (n > 0)` would not execute, yielding a count of 0, which is incorrect. A simple fix is to initialize the count to 1 if `n == 0`. For positive numbers, the loop runs `floor(log10(n)) + 1` times, which is the number of digits. For example, `n = 12345` requires 5 divisions (12345→1234→123→12→1→0). The time complexity is O(number of digits) = O(log10(n)), and auxiliary space is O(1). The function should not alter the caller's variable, so we either pass by value (which copies) or declare the parameter as `const int` and use a local copy. Using pass-by-value is simplest and ensures no side effects.

// Count the number of decimal digits in a non-negative integer.
// Returns 1 for n == 0.
int countDigits(int n) {
    if (n == 0) {
        return 1;
    }
    int count = 0;
    while (n > 0) {
        n /= 10;
        ++count;
    }
    return count;
}

#include <cassert>

int main() {
    assert(countDigits(0) == 1);
    assert(countDigits(5) == 1);
    assert(countDigits(9) == 1);
    assert(countDigits(10) == 2);
    assert(countDigits(99) == 2);
    assert(countDigits(100) == 3);
    assert(countDigits(12345) == 5);
    assert(countDigits(1000000) == 7);
    assert(countDigits(2147483647) == 10);
    assert(countDigits(1) == 1);
    return 0;
}
