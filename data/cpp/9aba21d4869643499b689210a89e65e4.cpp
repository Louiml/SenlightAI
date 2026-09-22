// Write a C++ function `int sumDigits(int number)` that accepts an integer between 0 and 1000 (inclusive) and returns the sum of its decimal digits. For example, `sumDigits(234)` returns `9` (because 2+3+4=9), and `sumDigits(7)` returns `7`. The input is guaranteed to be within the specified range, but your function should still handle edge cases like `0` and `1000` correctly. In your solution, do not use loops or recursion; only use arithmetic operations and explicit digit extraction. Provide a `const`-correct free function with a descriptive name that matches the specification.

#include <cassert>

int main() {
    assert(sumDigits(0) == 0);
    assert(sumDigits(5) == 5);
    assert(sumDigits(9) == 9);
    assert(sumDigits(10) == 1);
    assert(sumDigits(99) == 18);
    assert(sumDigits(123) == 6);
    assert(sumDigits(234) == 9);
    assert(sumDigits(999) == 27);
    assert(sumDigits(1000) == 1);
    assert(sumDigits(567) == 18);
    return 0;
}

#include <cstdlib>  // for std::abs if needed, but not required here since input is non-negative

// Sums the digits of an integer between 0 and 1000 inclusive.
// Uses only arithmetic operations; no loops or recursion.
int sumDigits(int number) {
    // Extract ones digit
    const int ones = number % 10;
    number /= 10;

    // Extract tens digit
    const int tens = number % 10;
    number /= 10;

    // Extract hundreds digit
    const int hundreds = number % 10;
    number /= 10;

    // Extract thousands digit (may be 0 for numbers < 1000)
    const int thousands = number % 10;

    return thousands + hundreds + tens + ones;
}

// The solution extracts each decimal digit from the given number using modulo (`%`) and integer division (`/`) operations. Since the maximum value is 1000, we need to handle up to 4 digits (for 1000, the digits are 1, 0, 0, 0). The algorithm works as follows:  
// - Extract the last digit by computing `number % 10` and store it.  
// - Remove that digit by integer division `number / 10`.  
// - Repeat the process for the remaining digits. Because the input is limited to 1000, we can simply do this a fixed number of times (e.g., four times for a 4-digit number). But careful: after processing the hundreds and tens digits, the final quotient may be zero, and extracting it would add 0 to the sum—that is safe. So we can safely extract and add the digit for each of the 4 positions (thousands, hundreds, tens, ones) regardless of input size.  
// Edge cases:  
// - `0` → digits are all 0 → sum is 0.  
// - `1000` → digits are 1, 0, 0, 0 → sum is 1.  
// - Single-digit numbers like `5` → sum is 5.  
// Time complexity is O(1) because we perform a fixed number of operations (4 iterations or explicit arithmetic). Space complexity is O(1), using only a few integer variables.
