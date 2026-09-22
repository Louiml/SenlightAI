/*
Write a C++ function `minimumRoundUpSum` that takes a `std::vector<long long>` containing exactly 5 positive integers (each ≤ 10^9) and returns the minimum possible total after applying the following operation exactly once: choose **one** of the five numbers to leave unchanged, and for each of the other four numbers, round it **up** to the nearest multiple of 10 (e.g., 13 → 20, 20 → 20, 8 → 10). The goal is to minimize the final sum. The function must return the minimal achievable sum as a `long long`. The input vector is guaranteed to have size 5, and all elements are positive. You may assume no element is zero. If all numbers are already multiples of 10, any one can be chosen, but the result is the same.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimal sum after selecting one number to keep and rounding up the rest.
long long minimumRoundUpSum(const std::vector<long long>& a) {
    // a is guaranteed to have exactly 5 elements.
    int skipIndex = 0;
    long long maxIncrement = 0;
    
    // Find the index with the largest rounding increment.
    for (int i = 0; i < 5; ++i) {
        long long increment = (10 - (a[i] % 10)) % 10;
        if (increment > maxIncrement) {
            maxIncrement = increment;
            skipIndex = i;
        }
    }
    
    long long total = 0;
    for (int i = 0; i < 5; ++i) {
        if (i == skipIndex) {
            total += a[i];  // Leave unchanged.
        } else {
            total += ((a[i] + 9) / 10) * 10;  // Round up to nearest multiple of 10.
        }
    }
    return total;
}

#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Example from the snippet: 5 numbers, minimal sum.
    assert(minimumRoundUpSum({3, 1, 4, 1, 5}) == 3 + 10 + 10 + 10 + 10); // 43
    assert(minimumRoundUpSum({10, 20, 30, 40, 50}) == 150);
    assert(minimumRoundUpSum({1, 2, 3, 4, 5}) == 1 + 10 + 10 + 10 + 10); // 41
    assert(minimumRoundUpSum({19, 29, 39, 49, 59}) == 19 + 30 + 40 + 50 + 60); // 199
    assert(minimumRoundUpSum({100, 101, 102, 103, 104}) == 100 + 110 + 110 + 110 + 110); // 540
    assert(minimumRoundUpSum({5, 5, 5, 5, 5}) == 5 + 10 + 10 + 10 + 10); // 45
    assert(minimumRoundUpSum({11, 12, 13, 14, 15}) == 11 + 20 + 20 + 20 + 20); // 91
    assert(minimumRoundUpSum({91, 92, 93, 94, 95}) == 91 + 100 + 100 + 100 + 100); // 491
    assert(minimumRoundUpSum({2, 8, 12, 18, 22}) == 2 + 10 + 20 + 20 + 30); // 82
    assert(minimumRoundUpSum({10, 11, 12, 13, 14}) == 10 + 20 + 20 + 20 + 20); // 90
    return 0;
}

// The key insight is that rounding up a number to the next multiple of 10 increases it by `(10 - (n % 10)) % 10`, which is 0 if the number already ends in 0, otherwise it adds the difference to the next 10. To minimize the total sum, we should leave unchanged the number whose rounding would add the **largest** extra cost, because we must leave exactly one number unchanged. But wait — we can only skip rounding one number. Therefore, among all five numbers, we compute the rounding increment for each. We should skip the one with the **maximum** increment, because that avoids the biggest penalty. If multiple numbers have the same maximum increment, pick any (e.g., the first). For each of the other four, we add the rounded-up value. The algorithm: initialize `maxGain` = 0 and `skipIndex` = 0; loop over all 5, compute `inc = (10 - (a[i] % 10)) % 10`; if `inc > maxGain`, update `maxGain` and `skipIndex`. Then sum all `a[i]` except for `skipIndex` where we add `a[skipIndex]` as is, and for others add `((a[i] + 9) / 10) * 10`. Edge cases: numbers already multiples of 10 have inc = 0, so if all are multiples, maxGain stays 0 and we skip index 0, but sum is all original numbers anyway because rounding them changes nothing. Time complexity O(5) = O(1), space O(1) auxiliary.
