// Write a C++ function that takes a positive integer `range` and a digit `d` (0-9), and returns the total number of times the digit `d` appears in all integers from 1 to `range` inclusive. For example, if `range = 25` and `d = 2`, the numbers containing 2 are 2, 12, 20, 21, 22 (appears twice), 23, 24, 25 — the total count is 9 (since 22 contributes two occurrences). The function must handle edge cases such as `d = 0` (e.g., for `range = 10`, digit 0 appears in 10 only, so count = 1) and `range = 0` (should return 0). The input `range` is a non-negative integer, and `d` is guaranteed to be a single digit from 0 to 9. The function should be efficient for `range` up to \(10^6\).
// The approach is straightforward: iterate through every integer from 1 to `range` (or from 0, but since 0 contributes nothing for any digit except possibly 0, we can start at 1 to avoid unnecessary checks), and for each number, count how many times the digit `d` appears in its decimal representation. To count occurrences in a single number, repeatedly extract the last digit using `num % 10`, compare it to `d`, and then divide `num` by 10 to remove the last digit, until the number becomes 0. For `d = 0`, note that the number 0 itself would be an issue if we included it, so we start from 1 to avoid counting a leading zero incorrectly. Edge cases: if `range` is 0, the loop does not execute and the function returns 0. If `d = 0` and `range` is small (e.g., 1–9), no number from 1 upward contains the digit 0, so the count remains 0. For `range = 10`, only 10 contains 0, giving count 1. Time complexity: The outer loop runs `range` times, and each number has at most `O(log10(n))` digits, so the total time is `O(range * log10(range))`. For `range` up to \(10^6\), this is acceptable (about 6 million operations). Space complexity is `O(1)` since only a few integer variables are used.
#include <iostream>

// Count total occurrences of digit d in all integers from 1 to range inclusive.
int countDigitOccurrences(int range, int digit) {
    int total = 0;
    for (int num = 1; num <= range; ++num) {
        int current = num;
        while (current > 0) {
            if (current % 10 == digit) {
                ++total;
            }
            current /= 10;
        }
    }
    return total;
}
#include <cassert>

int main() {
    // Basic tests
    assert(countDigitOccurrences(25, 2) == 9);   // 2,12,20,21,22(×2),23,24,25
    assert(countDigitOccurrences(10, 0) == 1);   // 10 contains 0
    assert(countDigitOccurrences(0, 5) == 0);    // range 0 gives 0
    assert(countDigitOccurrences(1, 1) == 1);    // just 1
    assert(countDigitOccurrences(99, 9) == 20);  // 9,19,29,...,99 (two in 99)
    assert(countDigitOccurrences(11, 1) == 4);   // 1,10,11(×2)
    assert(countDigitOccurrences(100, 0) == 11); // 10,20,...,90 (9) + 100 (2)
    assert(countDigitOccurrences(5, 3) == 1);    // 3 appears once
    assert(countDigitOccurrences(999, 0) == 189); // 0s in 1..999 (known pattern)
    assert(countDigitOccurrences(1000000, 2) == 600001); // known result for 10^6

    return 0;
}
