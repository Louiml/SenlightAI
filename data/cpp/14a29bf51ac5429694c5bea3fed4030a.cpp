You are given a rectangular sheet of paper with integer width `w` and height `h`. You want to cut it into `n` identical square pieces by repeatedly cutting any current rectangle exactly in half along its longer side (or either side if both are equal), and you may choose the orientation at each step. However, an alternative allowed operation is to make a single straight cut that divides a rectangle into two rectangles of equal area but not necessarily equal dimensions; for this task, the only allowed operation is repeatedly cutting a rectangle into two rectangles of equal area by slicing parallel to one of its sides, and you may choose which side to slice. Write a C++ function named `canMakeSheets` that takes three integers `w`, `h`, and `n` and returns `true` if it is possible to obtain exactly `n` sheets (each sheet being a rectangle of arbitrary dimensions) after performing zero or more such halving cuts, and `false` otherwise. The function must handle up to 10^4 test cases efficiently, and each of `w`, `h`, `n` can be up to 10^9.
#include <cassert>

int main() {
    // Basic cases
    assert(canMakeSheets(2, 3, 1) == true);
    assert(canMakeSheets(2, 3, 2) == true);
    assert(canMakeSheets(2, 3, 4) == true);
    assert(canMakeSheets(2, 3, 3) == false);

    // Exact power-of-two requirement
    assert(canMakeSheets(4, 1, 3) == false);
    assert(canMakeSheets(4, 1, 4) == true);
    assert(canMakeSheets(4, 1, 8) == true);
    assert(canMakeSheets(4, 1, 16) == false);

    // Odd dimensions
    assert(canMakeSheets(5, 7, 1) == true);
    assert(canMakeSheets(5, 7, 2) == false);

    // Large values
    assert(canMakeSheets(1024, 1024, 1024) == true);
    assert(canMakeSheets(1024, 1024, 2048) == true);
    assert(canMakeSheets(1024, 1024, 4096) == true);
    assert(canMakeSheets(1024, 1024, 8192) == false);

    // One dimension zero? Not allowed, but test with 1
    assert(canMakeSheets(1, 8, 32) == false);
    assert(canMakeSheets(1, 8, 16) == false);
    assert(canMakeSheets(1, 8, 8) == true);

    // Mixed powers
    assert(canMakeSheets(6, 10, 4) == true);
    assert(canMakeSheets(6, 10, 8) == true);
    assert(canMakeSheets(6, 10, 16) == false);
    assert(canMakeSheets(6, 10, 2) == true);

    return 0;
}
#include <cstdint>

// Determine if exactly n rectangles can be obtained from a w x h sheet
// by repeatedly slicing a rectangle into two equal-area halves.
bool canMakeSheets(int w, int h, long long n) {
    if (n == 1) {
        return true;
    }
    // n must be a power of two, because each cut doubles the count.
    if (n < 1 || (n & (n - 1)) != 0) {
        return false;
    }

    long long maxPieces = 1;
    int currentW = w;
    while (currentW % 2 == 0) {
        maxPieces *= 2;
        currentW /= 2;
    }

    int currentH = h;
    while (currentH % 2 == 0) {
        maxPieces *= 2;
        currentH /= 2;
    }

    return n <= maxPieces;
}
// The key observation is that each cut doubles the total number of pieces. Starting with 1 piece, after each cut you can increase the count by at most a factor of 2. But because you can choose the orientation at each cut, the maximum number of pieces you can obtain from a rectangle of width `w` and height `h` is `2^a * 2^b`, where `a` is the number of times `w` is divisible by 2, and `b` is the number of times `h` is divisible by 2. More precisely, you can repeatedly halve the width while it is even; each halving doubles the piece count. Similarly, halving the height while it is even doubles the count. The total maximum number of pieces is therefore `2^(v2(w) + v2(h))`, where `v2(x)` is the exponent of 2 in the prime factorization of `x`. Because you can always choose to stop cutting early, you can achieve any number of pieces that is a power of 2 up to that maximum, but not any non-power-of-2 count. Since `n` must be exactly achievable, the condition is simply that `n` is a power of 2 and `n <= 2^(v2(w) + v2(h))`. Equivalently, multiply the count by 2 for each factor of 2 in `w` and `h`, and check if `n <= count` and `(n & (n-1)) == 0` (i.e., `n` is a power of 2). Edge cases: if `n == 1`, always possible because no cuts needed. Time complexity O(log w + log h) per test case, space O(1). The original snippet incorrectly multiplied `ans` only by 2 for each halving but did not check that `n` is a power of 2; for example, `w=2, h=3, n=2` returns true but `n=3` would incorrectly return false if not power-of-2 checked, but the condition `n <= ans` alone would wrongly return true for `n=3` because `ans` becomes 2, and 3>2 false, so it happens to work, but consider `w=4, h=1, n=3`: `ans` becomes 8, and `3<=8` true, but you cannot get exactly 3 pieces since every cut doubles the count. Thus the correct condition must require `n` to be a power of 2.
