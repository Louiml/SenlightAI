// Write a C++ function named `printRangeFromNegToPos` that takes an integer `n` as input and returns a string containing all integers from `-n` to `n` (inclusive), in increasing order, separated by a single tab character (`\t`). The function should handle any integer `n`, including `0`, negative values, and very large values gracefully. For `n = 0`, the output should be just `"0"`. For negative `n`, the range should still be from `-n` to `n`, meaning that if `n` is negative, the range reverses mathematically (e.g., if `n = -3`, the output should be `"3 2 1 0 -1 -2 -3"`). The returned string should have no leading or trailing whitespace except the natural separators between numbers. The function must not print anything to the console; it must only return the constructed string.
The core algorithm is straightforward: iterate from `-n` to `n` inclusive, appending each integer to a string with a tab separator between consecutive values. The main loop uses an integer `i` that starts at `-n` and increments until it reaches `n`. For `n` positive or zero, this yields an increasing sequence. For `n` negative, `-n` is positive and `n` is negative, so the loop runs from a positive to a negative value, producing the reversed range. For `n = 0`, the loop executes once with `i = 0`. We use `std::to_string` to convert each integer to a string, and we carefully add a tab only before appending numbers that are not the first. Edge cases: when `n` is very large (e.g., near `INT_MAX`), we must ensure `-n` does not overflow; we use `long long` for the loop variable to safely handle the full range of `int` inputs, including `INT_MIN`. The time complexity is O(2n+1) = O(n), since we iterate over all integers in the range. The space complexity is O(1) auxiliary, not counting the output string itself, which grows proportionally to the number of integers (O(n) characters).
#include <string>
#include <cstdint>

// Returns a string with all integers from -n to n inclusive, tab-separated.
// Handles negative n by iterating from -n (positive) down to n (negative).
std::string printRangeFromNegToPos(int n) {
    std::string result;
    // Use long long to avoid overflow when negating INT_MIN.
    long long start = -static_cast<long long>(n);
    long long end = n;
    bool first = true;
    for (long long i = start; i <= end; ++i) {
        if (!first) {
            result += '\t';
        }
        result += std::to_string(i);
        first = false;
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above.
int main() {
    // Positive n
    assert(printRangeFromNegToPos(3) == "-3\t-2\t-1\t0\t1\t2\t3");
    // Zero
    assert(printRangeFromNegToPos(0) == "0");
    // Negative n
    assert(printRangeFromNegToPos(-2) == "2\t1\t0\t-1\t-2");
    // One
    assert(printRangeFromNegToPos(1) == "-1\t0\t1");
    // Larger value
    assert(printRangeFromNegToPos(5) == "-5\t-4\t-3\t-2\t-1\t0\t1\t2\t3\t4\t5");
    // INT_MIN edge case (should not overflow)
    assert(printRangeFromNegToPos(-3) == "3\t2\t1\t0\t-1\t-2\t-3");
    return 0;
}
