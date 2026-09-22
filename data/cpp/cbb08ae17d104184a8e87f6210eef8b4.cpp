// Write a C++ function `keyboardPressCount` that takes a positive integer `x` and returns the total number of key presses required to type all integers from 1 to `x` on a classic mobile phone keypad, where each digit key must be pressed a number of times equal to its position on the key (e.g., digit 1 requires 1 press, digit 2 requires 2 presses, ..., digit 9 requires 9 presses, and digit 0 requires 1 press). However, the given problem simplifies the calculation: the total number of presses for all numbers from 1 to `x` is computed as `(first_digit_of_x - 1) * 10 + (number_of_digits_in_x * (number_of_digits_in_x + 1)) / 2`. Your function must implement exactly this formula, handling any positive integer `x` (including those with leading digits from 1 to 9), and return the result as an integer.

The problem reduces to extracting the most significant digit (the first digit) and the total number of digits of `x`. The formula provided is a known shortcut for summing key presses for a range of numbers that share the same digit length. The first part, `(first_digit - 1) * 10`, accounts for all full sequences of numbers with fewer digits than `x` (each length contributes 10 total presses per starting digit: 1+2+...+9+1 for the 0). The second part, `(countDigit * (countDigit + 1)) / 2`, is the sum of the first `countDigit` natural numbers, representing the number of presses needed for each digit length of numbers up to the most significant digit. To find the first digit, repeatedly divide `x` by 10 until only one digit remains, while counting the divisions. Edge cases: if `x` is a single digit, `countDigit` is 1 and `first_digit` is `x` itself, so the result is `(x-1)*10 + 1`. Since `x` is positive, no zero or negative inputs need handling. The algorithm runs in O(d) time where d is the number of digits (at most about 19 for a 64-bit integer), and uses O(1) auxiliary space.

#include <cstdint>

// Returns the total number of key presses needed to type all numbers from 1 to x
// according to the simplified formula: (first_digit - 1) * 10 + (digit_count * (digit_count + 1)) / 2
int64_t keyboardPressCount(int64_t x) {
    int64_t first_digit = x;
    int digit_count = 0;
    
    // Extract the most significant digit and count the number of digits.
    while (first_digit >= 10) {
        first_digit /= 10;
        ++digit_count;
    }
    ++digit_count; // account for the final digit
    
    // Apply the formula.
    return (first_digit - 1) * 10 + (digit_count * (digit_count + 1)) / 2;
}

#include <cassert>
#include <cstdint>

// Declaration of the solution function (assumed to be included from the solution).
int64_t keyboardPressCount(int64_t x);

int main() {
    // Single-digit numbers
    assert(keyboardPressCount(1) == 1);
    assert(keyboardPressCount(5) == 41);
    assert(keyboardPressCount(9) == 81);
    
    // Two-digit numbers
    assert(keyboardPressCount(10) == 91);   // (1-1)*10 + 2*3/2 = 0 + 3 = 3? Wait, correct: 0 + 3 = 3? No, formula: (1-1)*10 + (2*3)/2 = 0 + 3 = 3? That's wrong—check: first_digit=1, count=2, so answer is 0+3=3, but actual key presses for 1..10 is 1+2+...+9+1 (for 1) + 1 (for 0) = 46+1? Let's trust formula. For x=10, first_digit=1, count=2 => 0 + 3 = 3? That can't be right. Re-evaluate: The original snippet used int x, and formula worked for cases like x=13 gave (1-1)*10 + (2*3)/2 = 0+3=3 which is wrong. Actually the snippet outputs 3 for x=13? No, the original loop: digit becomes 1, count=2, output (1-1)*10 + 3 = 3, but that's incorrect for real problem. However, as the task is defined to follow this exact formula, we test accordingly. For x=10, it's 3; but testing that would fail logical expectation. Since the task says "implement exactly this formula", we test computed values, not real-world correctness. Let's compute expected: x=10 -> first_digit=1, count=2 -> 0 + 3 = 3. x=99 -> first=9, count=2 -> 8*10+3=83. x=100 -> first=1, count=3 -> 0+6=6. x=123 -> first=1, count=3 -> 0+6=6. x=500 -> first=5, count=3 -> 4*10+6=46. So we'll assert those.
    assert(keyboardPressCount(10) == 3);
    assert(keyboardPressCount(99) == 83);
    assert(keyboardPressCount(100) == 6);
    assert(keyboardPressCount(123) == 6);
    assert(keyboardPressCount(500) == 46);
    
    // Three-digit and larger
    assert(keyboardPressCount(999) == 8*10 + 6); // first=9, count=3 => 80+6=86
    assert(keyboardPressCount(1000) == 0 + 10); // first=1, count=4=>0+10=10
    assert(keyboardPressCount(9876) == 8*10 + (4*5)/2); // first=9? Actually first digit of 9876 is 9, count=4 => 80+10=90
    
    // Large number
    assert(keyboardPressCount(1000000000000000000LL) == 0 + (19*20)/2); // first=1, count=19 => 190
    
    return 0;
}
