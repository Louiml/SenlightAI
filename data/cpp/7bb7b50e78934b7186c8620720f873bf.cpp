// Write a C++ function named `countDigits` that takes a non-negative integer as input and returns the number of digits in its decimal representation. The function should handle the edge case where the input is 0, which has exactly one digit. The input is guaranteed to be a non-negative integer, but may be as large as fits in an `int`. Your function should not use any library functions like `std::to_string` or logarithms; instead, it must use a simple loop that repeatedly divides the number by 10. If the input is 0, return 1 directly. For any other positive integer, count the number of divisions until the number becomes 0.

// The core idea is to repeatedly remove the last digit by integer division by 10, incrementing a counter each time. For any positive integer, the loop continues until the number becomes 0, and the counter equals the number of digits. The only edge case is when the input is 0, because the loop condition `num > 0` would not execute, yielding a count of 0, which is incorrect. Therefore, we explicitly check if the input is 0 and return 1. The algorithm runs in O(d) time, where d is the number of digits in the number. Since the input is an `int`, d is at most 10 (for 32-bit integers), so the time is effectively constant. Space complexity is O(1) because only a few integer variables are used.

#include <cstddef> // for std::size_t (optional, but good for counting)

// Count the number of digits in a non-negative integer.
// Returns 1 for input 0, otherwise counts divisions by 10.
std::size_t countDigits(int num) {
    if (num == 0) {
        return 1;
    }
    
    std::size_t digitCount = 0;
    while (num > 0) {
        num /= 10;
        ++digitCount;
    }
    return digitCount;
}

#include <cassert>

int main() {
    // Basic positive numbers
    assert(countDigits(0) == 1);
    assert(countDigits(1) == 1);
    assert(countDigits(9) == 1);
    assert(countDigits(10) == 2);
    assert(countDigits(99) == 2);
    assert(countDigits(100) == 3);
    assert(countDigits(12345) == 5);
    
    // Large numbers (for 32-bit int)
    assert(countDigits(2147483647) == 10);
    
    // Edge case: single digit zero
    assert(countDigits(7) == 1);
    
    // Ensure function is const-correct (no modification of parameter, but parameter is by value anyway)
    int val = 500;
    (void)val; // to avoid unused warning if compiled with -Wall
    
    // All checks pass
    return 0;
}
