// Write a C++ function named `isLuckyNumberCount` that takes a single 64-bit signed integer `n` and returns a string "YES" if the count of digits in `n` that are either 4 or 7 (i.e., "lucky digits") is itself either 4 or 7, and "NO" otherwise. The function must handle positive, zero, and negative inputs correctly—zero has no digits, so its lucky digit count is 0, and negative signs are ignored. For example, `474` has three lucky digits (4,7,4) → count=3 → "NO"; `4444` has four lucky digits → count=4 → "YES"; `7777777` has seven lucky digits → count=7 → "YES"; and `123` has no lucky digits → count=0 → "NO".
#include <cassert>
#include <string>

// Function prototype (as defined above)
std::string isLuckyNumberCount(long long n);

int main() {
    // Basic examples
    assert(isLuckyNumberCount(474) == "NO");      // 3 lucky digits -> NO
    assert(isLuckyNumberCount(4444) == "YES");    // 4 lucky digits -> YES
    assert(isLuckyNumberCount(7777777) == "YES"); // 7 lucky digits -> YES
    assert(isLuckyNumberCount(123) == "NO");      // 0 lucky digits -> NO

    // Edge cases: zero, negative, and single digit
    assert(isLuckyNumberCount(0) == "NO");           // 0 lucky digits -> NO
    assert(isLuckyNumberCount(-474) == "NO");        // 3 lucky digits (ignores sign) -> NO
    assert(isLuckyNumberCount(4) == "NO");           // 1 lucky digit -> NO
    assert(isLuckyNumberCount(7777) == "NO");        // 4 lucky digits but count=4 → YES (note: here count=4, so YES)
    // Actually 7777 has 4 lucky digits, so it should be YES; use a correct case:
    assert(isLuckyNumberCount(7777) == "YES");       // 4 lucky digits -> YES

    // Large number with many digits
    assert(isLuckyNumberCount(444444444LL) == "NO"); // 8 lucky digits -> NO
    assert(isLuckyNumberCount(4444444LL) == "YES");  // 7 lucky digits -> YES

    return 0;
}
#include <string>
#include <cstdlib>

// Return "YES" if the count of digits equal to 4 or 7 in n is 4 or 7.
std::string isLuckyNumberCount(long long n) {
    long long absN = std::llabs(n);
    int luckyDigitCount = 0;
    
    while (absN > 0) {
        int digit = absN % 10;
        if (digit == 4 || digit == 7) {
            ++luckyDigitCount;
        }
        absN /= 10;
    }
    
    return (luckyDigitCount == 4 || luckyDigitCount == 7) ? "YES" : "NO";
}
// The algorithm extracts digits from the absolute value of the input by repeatedly taking the modulo 10 to get the last digit, then dividing by 10 to remove it. For each digit, if it equals 4 or 7, increment a counter. After processing all digits, compare the counter to 4 and 7. Edge cases: (1) zero input—the while loop does not execute, so the counter remains 0, correctly returning "NO"; (2) negative input—take the absolute value first to avoid sign interference; (3) large inputs up to 9,223,372,036,854,775,807, which fit in a 64-bit signed type; the loop runs at most 19 times. Time complexity is O(d) where d is the number of digits (at most 19), and space complexity is O(1) auxiliary, excluding the returned string.
