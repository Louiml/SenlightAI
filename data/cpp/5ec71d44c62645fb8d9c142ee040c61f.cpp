// Write a C++ function named `digitalRoot` that accepts a non-negative integer (which may be very large, up to 1000 digits) as a `std::string` and returns its digital root as an `int`. The digital root is obtained by repeatedly summing the digits of the number until a single digit remains. For example, for `"942"` → 9+4+2=15 → 1+5=6, so the function should return `6`. The input string will contain only decimal digits and will not have leading zeros except for the number `"0"`, which should return `0`. Do not convert the entire string to an integer (it may overflow). Implement the function with `const` correctness and include all necessary headers.

// The solution works by first computing the sum of all digits in the string. Since the input length can be up to 1000 digits, the maximum digit sum is at most 9×1000 = 9000, which safely fits in an `int`. After obtaining this initial sum, we repeatedly apply the same digit-summation process until the result is a single digit (between 0 and 9). This is equivalent to computing the digital root via the formula `1 + (n-1) % 9` for positive `n`, but the iterative summation is clearer and avoids any modulo pitfalls with zero. Edge cases: the string `"0"` must return 0, and any other number will eventually reduce to a digit from 1 to 9. The algorithm runs in O(L) time for the first sum, and then at most a few iterations (since the sum quickly drops), so overall O(L) time. Space usage is O(1) beyond the input string.

#include <string>

// Computes the digital root of a non-negative integer given as a string.
// Repeatedly sums digits until a single digit remains.
int digitalRoot(const std::string& number) {
    int sum = 0;
    for (char c : number) {
        sum += c - '0';
    }
    while (sum >= 10) {
        int nextSum = 0;
        while (sum > 0) {
            nextSum += sum % 10;
            sum /= 10;
        }
        sum = nextSum;
    }
    return sum;
}

#include <cassert>

int main() {
    assert(digitalRoot("0") == 0);
    assert(digitalRoot("9") == 9);
    assert(digitalRoot("18") == 9);
    assert(digitalRoot("123") == 6);   // 1+2+3=6
    assert(digitalRoot("942") == 6);
    assert(digitalRoot("999999999") == 9);  // 9*9=81 → 8+1=9
    assert(digitalRoot("123456789") == 9);
    assert(digitalRoot("1") == 1);
    assert(digitalRoot("9876543210") == 9); // sum=45 → 4+5=9
    // Long 1000-digit number all '7's
    std::string longNum(1000, '7');
    // sum = 7000 → 7+0+0+0=7
    assert(digitalRoot(longNum) == 7);
}
