/*
Write a C++ function `int maximumSumAfterRemovingNegatives(const std::vector<int>& nums)` that accepts a vector of integers (possibly containing duplicates, zeros, and negative numbers) and returns the maximum possible sum achievable by selecting all positive numbers and, if no positive numbers exist, the largest (closest to zero) negative number. In other words, compute the sum of all unique positive values in the vector; if the vector contains no positive values, return the largest negative value (maximum of all negatives, i.e., the one with smallest absolute value). Zero is treated as neither positive nor negative, so if the vector contains only zeros and negatives, the result should be the largest negative value. If the vector contains at least one positive number, ignore all non‑positive values and return the sum of all distinct positive values only (do not count duplicates more than once). The function must handle an empty vector gracefully (though the input is promised non‑empty per typical constraints), returning 0 for an empty input.
*/

#include <vector>
#include <unordered_set>
#include <limits>
#include <numeric>

// Compute the maximum possible sum as per the problem description.
// - If any positive numbers exist: return the sum of all distinct positive numbers.
// - Else if any negative numbers exist: return the largest (closest to zero) negative number.
// - Else (only zeros or empty): return 0.
int maximumSumAfterRemovingNegatives(const std::vector<int>& nums) {
    std::unordered_set<int> positives;
    int largestNegative = std::numeric_limits<int>::min();
    bool hasNegative = false;

    for (int value : nums) {
        if (value > 0) {
            positives.insert(value);  // duplicates ignored by the set
        } else if (value < 0) {
            hasNegative = true;
            largestNegative = std::max(largestNegative, value);
        }
        // value == 0 is ignored
    }

    if (!positives.empty()) {
        // Sum all distinct positive values
        int sum = 0;
        for (int p : positives) {
            sum += p;
        }
        return sum;
    }

    if (hasNegative) {
        return largestNegative;
    }

    return 0;  // only zeros or empty input
}

#include <cassert>
#include <vector>

// Function declaration (or include the solution header)
int maximumSumAfterRemovingNegatives(const std::vector<int>& nums);

int main() {
    // Basic positive and negative mix
    assert(maximumSumAfterRemovingNegatives({1, -2, 3, -4}) == 4);   // 1 + 3 = 4

    // All negatives → return largest negative
    assert(maximumSumAfterRemovingNegatives({-5, -1, -3}) == -1);    // largest negative = -1

    // Mixed negatives and zeros, no positives → largest negative
    assert(maximumSumAfterRemovingNegatives({0, -2, 0, -7}) == -2);

    // Only positives, duplicates should be counted once
    assert(maximumSumAfterRemovingNegatives({2, 2, 3, 3, 3, 4}) == 9);  // 2+3+4 = 9

    // Single positive
    assert(maximumSumAfterRemovingNegatives({5}) == 5);

    // Single negative
    assert(maximumSumAfterRemovingNegatives({-8}) == -8);

    // Only zeros → return 0
    assert(maximumSumAfterRemovingNegatives({0, 0, 0}) == 0);

    // Positive, negative, zero, and duplicate positive
    assert(maximumSumAfterRemovingNegatives({10, -10, 0, 10, 2}) == 12); // 10+2 = 12

    // Empty vector (though not typical, test for safety)
    assert(maximumSumAfterRemovingNegatives({}) == 0);

    // All negative, with a larger negative and smaller negative
    assert(maximumSumAfterRemovingNegatives({-100, -50, -1, -2}) == -1);
    // Note: 0 is not positive, so if all negatives, largest negative is returned

    return 0;
}

// The core idea is to iterate through the input vector and use a hash set (`std::unordered_set<int>`) to collect distinct positive values and a separate variable to track the largest (maximum) negative value. Since the original code uses a map to count occurrences, but the task only requires distinct positive sum and the largest negative, a set suffices. For each element: if it is positive, insert it into the set (duplicates automatically ignored); if it is negative, update the `largestNegative` variable with `std::max`. After the loop, if the set is non‑empty, the answer is the sum of all elements in the set; otherwise, if `largestNegative` was updated (meaning at least one negative exists), return that value; if neither positive nor negative exist (only zeros or empty), return 0. Edge cases: a vector with only zeros → returns 0; a vector with negatives and zeros → returns largest negative; a vector with positives and negatives → returns sum of distinct positives; a vector with duplicates of positives → sum counts each distinct value once. Time complexity is O(n) average due to hash set operations, and O(n) worst‑case for hashing, with O(n) auxiliary space for the set (at most the number of distinct positives). The solution uses `const` reference to avoid copying and is const‑correct.
