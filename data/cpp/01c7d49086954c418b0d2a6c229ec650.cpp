// Write a C++ function named `sumOfDigits` that takes a non-negative integer `n` and returns the sum of its decimal digits. The function must handle `n = 0` correctly (returning 0), and should work for any non-negative value that fits in a standard `int`. The solution must not rely on string conversion; instead, it should use arithmetic operations to extract and accumulate each digit.

The core algorithm repeatedly extracts the last digit using the modulo operator (`n % 10`) and then removes that digit by integer division by 10 (`n / 10`), accumulating each extracted digit into a running total. This process continues until the number becomes zero. An important edge case is `n = 0`, because the loop condition `while (n > 0)` would skip the loop entirely, erroneously returning an initial sum of 0 — which is correct, but if we initialized the sum to 0, it works naturally. For positive numbers, the loop runs exactly the number of digits times. Since each iteration processes one digit, the time complexity is \(O(d)\), where \(d\) is the number of decimal digits (for an `int`, at most 10 digits). Auxiliary space usage is \(O(1)\), as only a few integer variables are needed. The function should be `const`-correct by taking the parameter by value (since we modify a local copy) and not changing any external state.

#include <cstddef> // not needed, but for completeness

// Return the sum of the decimal digits of a non-negative integer.
// Example: sumOfDigits(123) returns 6, sumOfDigits(0) returns 0.
int sumOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;   // add the last digit
        n /= 10;         // remove the last digit
    }
    return sum;
}

#include <cassert>

int main() {
    // Single-digit numbers
    assert(sumOfDigits(0) == 0);
    assert(sumOfDigits(5) == 5);
    assert(sumOfDigits(9) == 9);

    // Multi-digit numbers
    assert(sumOfDigits(123) == 6);
    assert(sumOfDigits(999) == 27);
    assert(sumOfDigits(1000) == 1);   // leading zeros don't affect sum
    assert(sumOfDigits(1024) == 7);

    // Large values
    assert(sumOfDigits(2147483647) == 46);  // 2+1+4+7+4+8+3+6+4+7 = 46
    assert(sumOfDigits(1000000000) == 1);

    return 0;
}
