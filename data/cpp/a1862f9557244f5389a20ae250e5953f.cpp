/*
Write a C++ function `std::string representSum(long long n)` that, given a positive integer \( n \), determines whether \( n \) can be expressed as the sum of three distinct positive integers \( a, b, c \) such that none of \( a, b, c \) is divisible by 3. If possible, return a string formatted as `"YES a b c"` where \( a < b < c \) (any valid triple is acceptable), and if impossible, return `"NO"`. Constraints: \( 1 \leq n \leq 10^{18} \). Examples: for \( n=7 \), output should be `"YES 1 2 4"` (since 1+2+4=7, none divisible by 3); for \( n=9 \), return `"NO"` (possible triples: 1+2+6 (6 divisible by 3), 1+3+5 (3 divisible), 2+3+4 (3 divisible), etc.); for \( n=12 \), return `"YES 1 4 7"`. Your function must handle large inputs efficiently without iterating over all triples.
*/
#include <string>

// Returns whether n can be expressed as sum of three distinct positive integers
// none divisible by 3, with an example triple or "NO".
std::string representSum(long long n) {
    if (n < 7 || n == 9) {
        return "NO";
    }
    if (n % 3 != 0) {
        // Choose 1, 2, n-3. Since n%3 != 0, n-3%3 != 0.
        return "YES 1 2 " + std::to_string(n - 3);
    } else {
        // n%3 == 0 and n != 9, so n >= 12. Choose 1, 4, n-5.
        // n-5 % 3 == 1, so not divisible by 3.
        return "YES 1 4 " + std::to_string(n - 5);
    }
}
#include <cassert>
#include <string>
#include <iostream>

// The solution function is declared above; here we test it.
int main() {
    assert(representSum(1) == "NO");
    assert(representSum(6) == "NO");
    assert(representSum(7) == "YES 1 2 4");
    assert(representSum(8) == "YES 1 2 5");
    assert(representSum(9) == "NO");
    assert(representSum(10) == "YES 1 2 7");
    assert(representSum(11) == "YES 1 2 8");
    assert(representSum(12) == "YES 1 4 7");
    assert(representSum(15) == "YES 1 4 10");
    assert(representSum(1000000000000000000LL) == "YES 1 4 999999999999999995");
    std::cout << "All tests passed.\n";
    return 0;
}
// The key observation is that any valid triple must consist of numbers not divisible by 3. The smallest such distinct positive integers are 1, 2, 4, 5, 7, 8, ... Their sums generate many values, but we need a systematic method. Notice:
// - If \( n < 7 \), impossible (minimum sum 1+2+4=7).
// - If \( n = 9 \), impossible (special case: the only possible triples with distinct positive integers not divisible by 3 are 1+2+6 (6 invalid), 1+3+5 (3 invalid), 1+4+4 (not distinct), 2+3+4 (3 invalid), so no solution).
// - For all other \( n \geq 7 \), a solution always exists. We can construct it:
//   - If \( n \% 3 \neq 0 \), choose \( a=1, b=2, c=n-3 \). Since \( n \not\equiv 0 \) mod 3, \( n-3 \equiv n \not\equiv 0 \), so \( c \) not divisible by 3. Also, for \( n \geq 7 \) and \( n \neq 9 \), we check \( c > b \) (i.e., \( n-3 > 2 \), which holds for \( n \geq 6 \)), and distinctness holds. For \( n=7 \), c=4, valid; \( n=10 \), c=7, valid; etc.
//   - If \( n \% 3 = 0 \) (and \( n \neq 9 \)): choose \( a=1, b=4, c=n-5 \). Since \( n \equiv 0 \) mod 3, \( n-5 \equiv 1 \) mod 3, so \( c \) not divisible by 3. For \( n \geq 12 \) (the next multiple of 3 after 9), \( c = n-5 \geq 7 > 4 \), so distinct and positive. Example: n=12 → 1+4+7=12.
// Special case \( n=9 \) returns "NO". Edge case \( n=7 \) falls under first case (7%3=1), returns 1 2 4. Time complexity O(1), space O(1) for constructed string.
