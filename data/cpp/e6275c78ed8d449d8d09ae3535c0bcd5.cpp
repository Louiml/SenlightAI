/*
Write a C++ function `lastDigitOfHyperSum` that takes a non-negative integer `n` (which may be extremely large, so it is provided as a string) and returns the last digit of the expression `1^n + 2^n + 3^n + 4^n`. The function should handle input strings up to 10^5 digits long. For example, if `n = 4`, then `1^4 + 2^4 + 3^4 + 4^4 = 1 + 16 + 81 + 256 = 354`, whose last digit is `4`. The function must compute the result efficiently without iterating over `n` or using big integers.
*/

#include <string>

// Returns the last digit of (1^n + 2^n + 3^n + 4^n) for a non-negative integer n given as a string.
// n may be arbitrarily large; only its remainder modulo 4 matters (except for n=0).
int lastDigitOfHyperSum(const std::string& n) {
    if (n == "0") {
        return 4; // 1^0 + 2^0 + 3^0 + 4^0 = 4
    }

    int lastTwoDigits = 0;
    if (n.size() >= 2) {
        lastTwoDigits = 10 * (n[n.size() - 2] - '0');
    }
    lastTwoDigits += n[n.size() - 1] - '0';

    int remainder = lastTwoDigits % 4;

    // Precomputed (k^n mod 10) for n mod 4 = 0,1,2,3 (for n>0)
    // k=1: 1,1,1,1
    // k=2: 6,2,4,8
    // k=3: 1,3,9,7
    // k=4: 6,4,6,4
    int ones[4] = {1, 1, 1, 1};
    int twos[4] = {6, 2, 4, 8};
    int threes[4] = {1, 3, 9, 7};
    int fours[4] = {6, 4, 6, 4};

    int sum = ones[remainder] + twos[remainder] + threes[remainder] + fours[remainder];
    return sum % 10;
}

#include <cassert>

int main() {
    assert(lastDigitOfHyperSum("0") == 4);
    assert(lastDigitOfHyperSum("1") == 0); // 1+2+3+4=10 last digit 0
    assert(lastDigitOfHyperSum("2") == 0); // 1+4+9+16=30 last digit 0
    assert(lastDigitOfHyperSum("3") == 0); // 1+8+27+64=100 last digit 0
    assert(lastDigitOfHyperSum("4") == 4); // 1+16+81+256=354 last digit 4
    assert(lastDigitOfHyperSum("5") == 0);
    assert(lastDigitOfHyperSum("100") == 4); // n mod 4=0
    assert(lastDigitOfHyperSum("12345678901234567890") == 0); // last two digits 90 => mod 4=2
    assert(lastDigitOfHyperSum("99999999999999999999") == 0); // last two digits 99 => mod 4=3
    assert(lastDigitOfHyperSum("100000000000000000000000000000000000000") == 4); // n mod 4=0
}

// The key observation is that the last digit of each term `k^n` depends only on `n mod 4` because for any base `k`, the sequence of `k^n mod 10` repeats with period 4 (since the multiplicative order of any digit modulo 10 divides 4). Specifically, for `k=1,2,3,4`, we can precompute the following table of `(k^n) mod 10` for `n mod 4` equal to 0,1,2,3:
// - `1^n mod 10` is always 1.
// - `2^n mod 10` cycles: 6,2,4,8 for n mod 4 = 0,1,2,3.
// - `3^n mod 10` cycles: 1,3,9,7.
// - `4^n mod 10` cycles: 6,4,6,4 (but actually `4^0=1`, but since n is non-negative and we consider mod 4, `4^0 mod 10=1`; however we can handle n=0 separately or use the cyclic table for n≥1; the sum `1^0+2^0+3^0+4^0=4` gives last digit 4, consistent with n mod 4=0 giving (1+6+1+6)=14 mod10=4, which matches if we define `4^0 mod10=1` and `2^0=1`, but `2^0 mod10=1` not 6; so careful: when n=0, `k^0=1` for all k. So we must handle n=0 as a special case).
// For n>0, the table above works. Alternatively, we can treat n mod 4 = 0 as using exponent 4, then the cycle works because `2^4 mod10=6`, `3^4 mod10=1`, `4^4 mod10=6`. So for n>0, the table is correct. For n=0, the result is 4. Thus the algorithm: if the input string is "0" return 4; else compute n mod 4 from the last two digits (or one digit if N==1) of the string, then sum the precomputed remainders and take mod 10. Time complexity O(1) after reading the string, space O(1).
