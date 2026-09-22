/*
Write a C++ function that takes a positive integer `n` as input and returns the sum of all even digits present in that number. For example, given `n = 2468`, the function should return `20` (2+4+6+8), and for `n = 13579`, the function should return `0` since there are no even digits. The function should handle edge cases such as a single-digit number (e.g., `n = 8` returns `8`, `n = 7` returns `0`) and very large integers (e.g., `1234567890` returns `20`). The input is guaranteed to be a non-negative integer, so you may assume no negative values, but still write the code robustly.
*/

#include <cstdint>

// Returns the sum of all even decimal digits in the non-negative integer n.
// E.g., sumEvenDigits(2468) == 20, sumEvenDigits(13579) == 0.
int sumEvenDigits(int64_t n) {
    int sum = 0;
    while (n != 0) {
        int digit = static_cast<int>(n % 10);
        if (digit % 2 == 0) {
            sum += digit;
        }
        n /= 10;
    }
    return sum;
}

#include <cassert>
#include <cstdint>

int sumEvenDigits(int64_t n); // Declaration for testing

int main() {
    assert(sumEvenDigits(0) == 0);
    assert(sumEvenDigits(8) == 8);
    assert(sumEvenDigits(7) == 0);
    assert(sumEvenDigits(2468) == 20);
    assert(sumEvenDigits(13579) == 0);
    assert(sumEvenDigits(1234567890) == 20);
    assert(sumEvenDigits(1002) == 2);
    assert(sumEvenDigits(222) == 6);
    assert(sumEvenDigits(111) == 0);
    assert(sumEvenDigits(987654321) == 20); // even digits: 8+6+4+2 = 20
    return 0;
}

// The solution repeatedly extracts the last digit of the number using the modulo operator (`% 10`) and checks if that digit is even (`digit % 2 == 0`). If it is even, we add it to an accumulator sum. Then we remove the last digit by integer division by 10 (`/= 10`). This process repeats until the number becomes zero. Since we process each decimal digit exactly once, the time complexity is O(d), where d is the number of digits (approximately O(log₁₀ n)). The auxiliary space used is O(1) — only a few scalar variables. Edge cases include `n = 0`, where the loop doesn’t execute (since `while (n != 0)` fails immediately) and the sum remains 0, which is correct because 0 is even but has no digits contributing? Actually 0 is even but we treat 0 as having no digits, so return 0. A single-digit even number works because the loop runs once. A number like `1002` gives even digits 0,0,2 — but note the leading zeros inside the number are real digits, so we correctly include them: for `1002`, digits are 1,0,0,2, sum of evens = 0+0+2 = 2. The algorithm is straightforward and requires no extra data structures.
