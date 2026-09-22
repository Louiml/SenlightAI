/*
Write a C++ function named `countDigits` that takes a non-negative integer as input and returns the number of digits in its decimal representation. The function must handle the edge case of zero correctly (zero has 1 digit). The solution must use an iterative division-based approach (repeatedly dividing by 10) to count digits, and must not rely on converting the number to a string or using logarithmic functions. The function signature must be `int countDigits(int n)`.
*/

#include <cstddef>

// Count the number of digits in a non-negative integer using repeated division.
int countDigits(int n) {
    // Handle zero explicitly: it has exactly one digit.
    if (n == 0) {
        return 1;
    }
    
    int digitCount = 0;
    // Repeatedly divide by 10 until n becomes zero.
    while (n > 0) {
        ++digitCount;
        n /= 10;
    }
    return digitCount;
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
    assert(countDigits(100000) == 6);
    assert(countDigits(2147483647) == 10); // INT_MAX
    assert(countDigits(1000000000) == 10);
    return 0;
}

// The main algorithm is straightforward: repeatedly divide the input number by 10 and increment a counter until the number becomes zero. For the edge case `n == 0`, the loop body never executes (since `0 > 0` is false), so we must initialize the counter to 1 to correctly return 1 digit for zero. For any positive integer, the loop runs exactly as many times as there are digits. Time complexity is \(O(\text{number of digits}) = O(\log_{10} n)\), and space complexity is \(O(1)\). The function should use a local variable for the counter, apply `const` to the parameter if possible (though the parameter is passed by value, we can make a copy), and avoid modifying the original input.
