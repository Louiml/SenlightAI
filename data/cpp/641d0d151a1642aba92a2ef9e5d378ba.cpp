Write a C++ function `bool isDivisibleBySix(int G)` that takes a non-negative integer `G` and returns `true` if `G` is divisible by 6, and `false` otherwise. The function must be implemented using only modular arithmetic properties that avoid directly computing the full division (e.g., by checking divisibility by 2 and 3 separately via digit sums and last digit, or by using the fact that `G % 6` can be derived from `G % 2` and `G % 3`). The solution should work for any non-negative integer up to the maximum value of `int`. The accompanying `main` function (provided in the test section) will read a single integer from standard input, call your function, and print `"Y"` if divisible by 6, else `"N"`. Edge cases include `0` (divisible by 6), very large integers near the `int` limit, and numbers with trailing zeros.
#include <cassert>

// Declaration for the solution function (assume it's in the same translation unit)
bool isDivisibleBySix(int G);

int main() {
    // Edge cases
    assert(isDivisibleBySix(0) == true);        // zero is divisible by 6
    assert(isDivisibleBySix(6) == true);        // smallest positive
    assert(isDivisibleBySix(1) == false);
    assert(isDivisibleBySix(5) == false);
    
    // Large numbers near int limit
    assert(isDivisibleBySix(2147483646) == false); // 2147483646 % 2 = 0, but %3 != 0
    assert(isDivisibleBySix(2147483640) == true);  // divisible by 6
    
    // Numbers with trailing zeros
    assert(isDivisibleBySix(120) == true);
    assert(isDivisibleBySix(100) == false);    // 100 % 2 == 0, 100 % 3 != 0
    
    // Random checks
    assert(isDivisibleBySix(18) == true);
    assert(isDivisibleBySix(21) == false);
    
    return 0;
}
#include <cstddef>

// Returns true if the given non-negative integer is divisible by 6.
// The implementation uses modular arithmetic; it checks divisibility by 2 and 3
// separately to avoid any direct division by 6, but the result is equivalent.
bool isDivisibleBySix(int G) {
    // Extracting the last digit (for divisibility by 2) and the digit sum (for divisibility by 3)
    // This approach is based on the property that a number is divisible by 6 iff divisible by 2 and 3.
    
    // Handle the absolute value in case negative numbers are passed (though the task specifies non-negative).
    int n = (G < 0) ? -G : G;
    
    // Divisibility by 2: the last digit must be even.
    bool divisibleBy2 = (n % 2 == 0);
    
    // Divisibility by 3: sum of digits modulo 3 must be zero.
    int digitSum = 0;
    while (n > 0) {
        digitSum += n % 10;
        n /= 10;
    }
    bool divisibleBy3 = (digitSum % 3 == 0);
    
    return divisibleBy2 && divisibleBy3;
}
// The simplest approach is to directly compute `G % 6` using the `%` operator, which is correct and efficient. However, to highlight the modular properties (inspired by the given snippet that uses `G % 6`), the solution can also check divisibility by 2 (last digit even) and by 3 (sum of digits divisible by 3), then return `true` only if both conditions hold. This avoids explicit division by 6 but still runs in O(number of digits) time, which is at most 10 digits for 32-bit `int`, so effectively O(1) time, and O(1) auxiliary space. Key edge cases: `0` is divisible by 6 (sum of digits 0, last digit 0 even), negative numbers are not considered but if they were, we'd take absolute value; large values near `INT_MAX` cause no overflow because digit sum fits easily. Alternative: use the property that `G % 6` equals `0` iff `G % 2 == 0` and `G % 3 == 0`, but the given snippet suggests a direct modulo, so we can also just return `G % 6 == 0`. For the solution function, we'll implement the direct modulo approach for clarity and correctness, while mentioning the alternative in comments. Time complexity O(1), space O(1).
