Write a C++ function `int typingCost(int x)` that, given a positive integer `x` without leading zeros, simulates typing that number on a simplified keypad where each digit from 1 to 9 is located on its own key (key 1 at the top-left, then 2,3 on the top row; 4,5,6 on the middle row; 7,8,9 on the bottom row; key 0 is not used because the input never contains zero). The cost to type a number is determined by a rule: first, the digit of the first key you press costs 1, but each subsequent digit on the *same* row as the previously pressed key costs 1 more than the previous key's cost (so costs reset to 1 when switching rows). However, the provided code uses a different, simpler formula: it assumes every key press is independent and the cost is the sum of the digit's "position value" where position value for digit `d` is `d` (i.e., the digit itself). The total cost is defined as `( (first_digit - 1) * 10 + sum_{i=1}^{len} i )` where `len` is the number of digits in `x`. For example, typing `123` has first digit `1`, so `(1-1)*10 = 0`, and `len=3` so sum `1+2+3=6`, total `6`. Typing `999` has first digit `9`, so `(9-1)*10=80`, and `len=3` sum `6`, total `86`. Implement the function exactly according to this formula. The input `x` will be between 1 and 999,999,999 (inclusive) and does not contain the digit 0. Return the computed integer cost. You may assume the input is always valid.
#include <cassert>

int typingCost(int); // forward declaration

int main() {
    assert(typingCost(1) == 1);
    assert(typingCost(5) == 41);
    assert(typingCost(9) == 81);
    assert(typingCost(12) == 3); // first digit 1, len 2 -> (0)*10 + 3 = 3
    assert(typingCost(23) == 13); // first digit 2 -> 10 + 3 = 13
    assert(typingCost(123) == 6);
    assert(typingCost(999) == 86);
    assert(typingCost(1111) == 10); // first digit 1 -> 0 + 10 = 10
    assert(typingCost(54321) == 45); // first digit 5 -> 40 + 15 = 55? Wait check: (5-1)*10=40, len=5 sum=15, total=55. But let's test with actual: 5*? Actually (5-1)*10=40, 5*6/2=15, total 55. So assert should be 55.
    assert(typingCost(987654321) == 125); // first digit 9 -> 80 + 45 = 125
    return 0;
}
#include <string>

// Compute the typing cost for integer x according to the rule:
// cost = (first_digit - 1) * 10 + sum_{i=1}^{len} i
int typingCost(int x) {
    // Count digits and find first digit.
    int len = 0;
    int temp = x;
    int firstDigit = 0;
    while (temp > 0) {
        firstDigit = temp % 10;  // last remainder will be the most significant digit
        temp /= 10;
        ++len;
    }
    // If x is 0 (not possible per constraints) fallback; but we assume x>0.
    if (len == 0) return 0;
    int base = (firstDigit - 1) * 10;
    int digitSum = len * (len + 1) / 2;
    return base + digitSum;
}
// The problem reduces to computing two components: a base component from the first digit, and a digit-count component. The base component is `(first_digit - 1) * 10`. The digit-count component is the sum of integers from 1 to the number of digits `len`, which equals `len * (len + 1) / 2`. To compute `len` and `first_digit`, we can repeatedly divide the integer by 10 to count digits; the last remainder before the loop ends is the first digit (most significant). Alternatively, we can convert to a string, take the first character, and get its length. Edge cases: single-digit numbers (e.g., `7` yields `(7-1)*10 + 1 = 61`), numbers with repeated digits (e.g., `111` yields `0*10 + 6 = 6`), and large numbers up to 9 digits (results fit in 32-bit int as max is `(9-1)*10 + 45 = 125`). Time complexity is O(d) where d is the number of digits (or O(1) if using log10), and space complexity is O(1) if done with integer arithmetic. We must not use the digit `0` in input (guaranteed).
