/*
Write a C++ function named `SumOfDigits` that accepts a single `int64_t` value (which may be negative, zero, or positive) and returns the sum of the absolute value of its decimal digits as an `int64_t`. For example, `1234` yields `10`, `-987` yields `24`, and `0` yields `0`. The function must handle the largest possible negative value (`INT64_MIN`) safely without causing undefined behavior due to overflow. You should implement the algorithm yourself (no use of `std::to_string` or string conversion) and ensure the function is `const`-correct and self-contained with proper includes.
*/

#include <cstdint>

// Returns the sum of the absolute decimal digits of the given number.
// Handles negative numbers and INT64_MIN without overflow.
int64_t SumOfDigits(int64_t number) {
    int64_t sum = 0;
    // Process all digits, including zero (loop does not run for zero).
    while (number != 0) {
        // For negative numbers, digit is negative; take its absolute value.
        int64_t digit = number % 10;
        sum += (digit < 0) ? -digit : digit;
        number /= 10;  // Truncates toward zero; safe for INT64_MIN.
    }
    return sum;
}

#include <cassert>
#include <cstdint>

// Declaration of the function under test (defined elsewhere).
int64_t SumOfDigits(int64_t number);

int main() {
    // Basic positive cases
    assert(SumOfDigits(0) == 0);
    assert(SumOfDigits(5) == 5);
    assert(SumOfDigits(1234) == 10);
    assert(SumOfDigits(100000) == 1);
    assert(SumOfDigits(999999999) == 81);

    // Negative cases
    assert(SumOfDigits(-5) == 5);
    assert(SumOfDigits(-123) == 6);
    assert(SumOfDigits(-987) == 24);
    assert(SumOfDigits(-100000) == 1);

    // Edge cases: INT64_MIN and INT64_MAX
    constexpr int64_t kMin = INT64_MIN;
    constexpr int64_t kMax = INT64_MAX;
    assert(SumOfDigits(kMin) == 80); // digits: 9,2,2,3,3,7,2,0,3,6,8,5,4,7,7,5,8,0,8
    assert(SumOfDigits(kMax) == 88); // digits: 9,2,2,3,3,7,2,0,3,6,8,5,4,7,7,5,8,0,7

    return 0;
}

// The core algorithm repeatedly extracts the least‑significant digit using the modulo operation with base 10 and adds it to a running total, then divides the number by 10 to shift right. The main challenge is handling negative inputs: if we simply loop `while (number > 0)`, negative numbers would return `0`. The standard trick is to work with the absolute value, but `std::abs(INT64_MIN)` overflows because `INT64_MIN` has magnitude `2^63` which is not representable in `int64_t`. To avoid overflow, we can process the number digit‑by‑digit while it is negative (or positive) without taking the absolute value upfront. Instead, we can repeatedly take `digit = number % 10` (which for negative numbers yields a negative digit) and accumulate its absolute value, then divide by 10. However, for `INT64_MIN`, `number % 10` is safe (result is -8) and `number / 10` is also safe (result is -922337203685477580), because division truncates toward zero and does not overflow. This works for all negative numbers. Alternatively, we can use a `uint64_t` copy of the absolute value computed safely by `static_cast<uint64_t>(-(number + 1)) + 1` for negatives, but the modulo/division approach on the signed negative directly is simpler and correct. The loop continues while `number != 0` (works for both negative and positive). Edge cases: `0` returns `0` (loop never runs). `INT64_MIN` works because the last digit is -8, then after successive divisions, the number becomes negative and eventually zero. Time complexity is O(d) where d is the number of decimal digits (at most 19 for int64), which is effectively O(1) for fixed‑width types. Space complexity is O(1).
