Write a C++ function `countDigitInFactorial` that takes a positive integer `n` (with 1 ≤ n ≤ 20) and a digit `d` (0 ≤ d ≤ 9), computes the factorial `n!`, and returns the number of occurrences of the digit `d` in the decimal representation of that factorial. The factorial may be very large (up to 20! = 2,432,902,008,176,640,000, which exceeds 64-bit range for n ≥ 21, but n ≤ 20 fits in `unsigned long long`; however, to be safe and match the original snippet’s big-integer approach, implement or use a simple arbitrary‑precision integer class). The function must avoid converting the factorial to a string first; instead, it should directly inspect the decimal digits by storing them in a digit array (least‑significant digit first). You may assume the input is always valid, but handle edge cases like `n = 1` (factorial = 1) and `d = 0` (which may appear inside numbers, e.g., 10! = 3628800 has two zeros). Return the count as an `int`. The function signature must be `int countDigitInFactorial(int n, int d)`.

#include <cassert>

// Function declaration (from solution)
int countDigitInFactorial(int n, int d);

int main() {
    // n=1: 1! = 1
    assert(countDigitInFactorial(1, 1) == 1);
    assert(countDigitInFactorial(1, 0) == 0);

    // n=2: 2! = 2
    assert(countDigitInFactorial(2, 2) == 1);
    assert(countDigitInFactorial(2, 1) == 0);

    // n=3: 3! = 6
    assert(countDigitInFactorial(3, 6) == 1);
    assert(countDigitInFactorial(3, 0) == 0);

    // n=4: 4! = 24 → digit 2 and 4
    assert(countDigitInFactorial(4, 2) == 1);
    assert(countDigitInFactorial(4, 4) == 1);

    // n=5: 5! = 120 → two zeros? Actually one zero
    assert(countDigitInFactorial(5, 0) == 1);
    assert(countDigitInFactorial(5, 1) == 1);
    assert(countDigitInFactorial(5, 2) == 1);

    // n=10: 10! = 3628800 → zeros: two (digits 8,8,0,0,2,6,3)
    assert(countDigitInFactorial(10, 0) == 2);
    assert(countDigitInFactorial(10, 3) == 1);
    assert(countDigitInFactorial(10, 6) == 1);
    assert(countDigitInFactorial(10, 2) == 1);
    assert(countDigitInFactorial(10, 8) == 2);

    // n=20: 20! = 2432902008176640000 → count digit 0 = 4? Let's verify: 
    // digits: 2 4 3 2 9 0 2 0 0 8 1 7 6 6 4 0 0 0 0 → zeros at positions: 
    // Actually 20! = 2432902008176640000 → zeros: three? Let's test known value:
    // 2432902008176640000 has zeros at the end? No, it ends with 0000? Actually 
    // 20! = 2432902008176640000 → yes, four zeros at the end, plus one more interior? 
    // The correct digit count for 0 is 4? Let's just assert a known result: 
    // 20! = 2432902008176640000 → digits: 2,4,3,2,9,0,2,0,0,8,1,7,6,6,4,0,0,0,0 
    // So zeros = 1 (after 9) + 1 (after 2) + 1 (after 0? wait) – better to compute by code but
    // I'll assert a known correct count: 20! has 19 digits, zeros appear at indices 5,7,8,15,16,17,18? 
    // That’s 7 zeros? Actually let's just use a known correct value: 
    // 20! = 2432902008176640000 → digit '0' appears 4 times? 
    // Let's count: 2 4 3 2 9 0 2 0 0 8 1 7 6 6 4 0 0 0 0 → zeros at positions 5,7,8,15,16,17,18 → that's 7! 
    // But we need to be accurate. To avoid error, I'll just compute using Python mentally? 
    // Safer to assert for n=20, d=0 equals 7? Let's check: 20! = 2432902008176640000
    // Digits: 2,4,3,2,9,0,2,0,0,8,1,7,6,6,4,0,0,0,0 → count zeros = 1 (index 5) + 1 (index 7) + 2 (index 8,9? 
    // Actually index 8 and 9 are '0' and '0'? Let me write: positions 0-18:
    // 0:2,1:4,2:3,3:2,4:9,5:0,6:2,7:0,8:0,9:8,10:1,11:7,12:6,13:6,14:4,15:0,16:0,17:0,18:0
    // That gives zeros at 5,7,8,15,16,17,18 → total 7.
    assert(countDigitInFactorial(20, 0) == 7);
    assert(countDigitInFactorial(20, 4) == 2); // 4 appears at indices 1 and 14 → 2
    assert(countDigitInFactorial(20, 2) == 3); // 2 appears at indices 0,3,6 → 3

    return 0;
}

#include <vector>
#include <cstdint>

// Count occurrences of digit d in the decimal representation of n! (1 ≤ n ≤ 20)
int countDigitInFactorial(int n, int d) {
    // Fixed-size digit array, least-significant digit first.
    // 20! has 19 digits, so 40 is safe.
    std::vector<int> digits(40, 0);
    digits[0] = 1;
    int len = 1; // current number of digits

    // Multiply by each integer from 2 to n
    for (int multiplier = 2; multiplier <= n; ++multiplier) {
        int carry = 0;
        for (int i = 0; i < len; ++i) {
            int prod = digits[i] * multiplier + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    // Count occurrences of d in the digit array (only first len digits)
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (digits[i] == d) {
            ++count;
        }
    }
    return count;
}

// The core of the problem is computing `n!` without overflow and then counting occurrences of a specific digit in its decimal representation. Since `n` is at most 20, the factorial is at most 20! ≈ 2.43×10^18, which fits within the 64-bit `unsigned long long` range (max ~1.8×10^19). However, to demonstrate a robust big-integer approach and avoid relying on built-in types, we can implement a small fixed‑size decimal digit array (e.g., 40 digits is enough for 20! which has 19 digits). The multiplication algorithm: initialize an array `digits[0] = 1`, `len=1`. For each multiplier from 2 to n, multiply the current number (stored least‑significant digit first) by that integer, handling carries manually. After computing the factorial, iterate through the digit array from index 0 to len-1 and count how many equal `d`. Edge cases: `n=1` gives factorial 1; `d=0` may appear inside numbers (e.g., 10! = 3628800 has two zeros); if the factorial is exactly 0 (never happens for n≥1), the digit count for 0 would be 1, but since n≥1, factorial > 0, and we ignore leading zeros that are not stored. Time complexity: O(n × L) where L is the current number of digits (at most 19), so effectively O(n^2) in the worst case, but for n ≤ 20 it’s constant. Space complexity: O(1) fixed array of 40 ints.
