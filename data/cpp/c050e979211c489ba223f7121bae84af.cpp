/*
Write a C++ function `longestCollatzSequenceLength(int start, int end)` that, given two positive integers `start` and `end` (where the order is not guaranteed, i.e., `start` may be greater than `end`), computes the maximum Collatz sequence length among all integers in the inclusive range between the two values. The Collatz sequence is defined as: for a number `n`, if `n` is even, the next term is `n / 2`; if `n` is odd, the next term is `3 * n + 1`; the sequence ends when it reaches `1`, and the length counts the number of terms including the starting number and the final `1`. For example, the sequence from 5 is 5→16→8→4→2→1, length 6. The function should return the maximum length found. Assume inputs are positive (≥1) and fit within `int` range, but note that intermediate Collatz values may exceed `int` (e.g., for large starting numbers) — use `long long` for internal computation to avoid overflow.
*/
#include <algorithm>

// Compute the length of the Collatz sequence starting at n (n >= 1).
// Uses long long to avoid overflow on intermediate odd steps (3*n+1).
int collatzLength(long long n) {
    int len = 1; // count the starting term
    while (n > 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        ++len;
    }
    return len;
}

// Return the maximum Collatz sequence length among all integers
// in the inclusive range between start and end (order not guaranteed).
int longestCollatzSequenceLength(int start, int end) {
    const int lo = std::min(start, end);
    const int hi = std::max(start, end);
    int maxLen = 0;
    for (int x = lo; x <= hi; ++x) {
        const int currentLen = collatzLength(static_cast<long long>(x));
        maxLen = std::max(maxLen, currentLen);
    }
    return maxLen;
}
int main() {
    // Basic cases
    assert(longestCollatzSequenceLength(1, 1) == 1);          // single 1
    assert(longestCollatzSequenceLength(2, 2) == 2);          // 2 -> 1
    assert(longestCollatzSequenceLength(5, 5) == 6);          // 5 -> 16 -> 8 -> 4 -> 2 -> 1
    // Range including order reversal
    assert(longestCollatzSequenceLength(10, 1) == 20);        // max among 1..10 is at 9 (length 20)
    assert(longestCollatzSequenceLength(1, 10) == 20);
    // Larger range
    assert(longestCollatzSequenceLength(1, 100) == 119);      // max at 97
    // Edge with adjacent numbers
    assert(longestCollatzSequenceLength(27, 27) == 112);      // known length for 27
    assert(longestCollatzSequenceLength(27, 28) == 112);      // 28 length is 19, 27 wins
    // Identical boundaries but reversed
    assert(longestCollatzSequenceLength(100, 50) == 119);     // 97 in range? No, 50..100 includes 97
    // Positive only, but very large start (fits int)
    assert(longestCollatzSequenceLength(999999, 999999) > 100);
    return 0;
}
// The solution iterates over every integer `x` in the inclusive range `[min(start, end), max(start, end)]`. For each `x`, it computes the Collatz sequence length using a helper function `collatzLength(long long n)` that loops until `n` reaches 1, incrementing a length counter each step. Since intermediate values can exceed the original `int` range, use `long long` for the working variable inside the helper. Track the maximum length across all numbers. Edge cases: when `start == end`, the range has one number; when one input is 1, its sequence length is 1; ensure the range is properly ordered by computing `lo = min(start, end)` and `hi = max(start, end)`. There is no need to cache results for this simple version, but if performance is a concern, memoization could reduce repeated computation (though not required here). Time complexity: for each number in the range of size `k = |start - end| + 1`, the Collatz sequence length is roughly logarithmic in the number, so the total is O(k * log(max_value)), but in practice the sequence length is small (typically < a few hundred). Space complexity: O(1) auxiliary space (excluding output), since we only store a few variables.
