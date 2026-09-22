Write a C++ function `bitParityDifferent(int N, int M)` that takes two non-negative integers \(N\) and \(M\) (with \(1 \le N, M \le 10^{18}\), so they fit in a 64-bit signed integer) and returns `1` if \(N\) and \(M` have different parity (one is even, the other is odd), otherwise returns `0`. The solution must use only bitwise operations (e.g., `&`, `|`, `^`, `~`) and no arithmetic, relational, or logical operators. The function should be declared as `int bitParityDifferent(long long N, long long M);` and must be `const`-correct (i.e., parameters passed by value can be marked `const`). Handle the full range of inputs correctly, including large values near \(10^{18}\).

#include <cassert>

// Assume bitParityDifferent is declared above.

int main() {
    // Even vs odd -> 1
    assert(bitParityDifferent(2, 3) == 1);
    assert(bitParityDifferent(0, 1) == 1);
    assert(bitParityDifferent(1000000000000000000LL, 999999999999999999LL) == 1);

    // Odd vs even -> 1
    assert(bitParityDifferent(7, 4) == 1);
    assert(bitParityDifferent(1, 2) == 1);

    // Even vs even -> 0
    assert(bitParityDifferent(2, 4) == 0);
    assert(bitParityDifferent(0, 100) == 0);

    // Odd vs odd -> 0
    assert(bitParityDifferent(3, 5) == 0);
    assert(bitParityDifferent(999999999999999999LL, 1) == 0);

    // Large equal odd
    assert(bitParityDifferent(999999999999999999LL, 999999999999999999LL) == 0);
}

#include <cstdint>

// Return 1 if N and M have different parity (one even, one odd), else 0.
// Uses only bitwise operations.
int bitParityDifferent(const long long N, const long long M) {
    // Isolate least significant bit for each: 1 if odd, 0 if even.
    // XOR gives 1 iff they differ.
    return static_cast<int>((N & 1LL) ^ (M & 1LL));
}

// The core idea is to extract the least significant bit of each number, which indicates its parity: a number is odd if its LSB is 1, even if 0. Using a bitwise AND with `1` (`N & 1`) isolates that bit. Then, XOR (`^`) of the two parity bits yields `1` when they differ (since XOR returns 1 for inputs 0 and 1) and `0` when they are the same. This exactly matches the required output. No loops or division are needed. Edge cases: both numbers can be 0 (though constraints say ≥1, the function should still work), both odd, both even, or different parities — the XOR handles all. Time complexity is O(1), space complexity is O(1), as only a few register operations are performed.
