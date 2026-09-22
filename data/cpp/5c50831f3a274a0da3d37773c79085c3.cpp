Write a C++ function that, given a non-empty vector of 32-bit integers, returns the total number of contiguous subarrays consisting entirely of zeros. For example, in the vector `{0,0,1,0}`, the valid zero-filled subarrays are `[0]`, `[0,0]`, `[0]` (the last element), so the answer is 4. The function must handle vectors of any length, including those with no zeros (returning 0), and must return a `long long` to accommodate large counts. Do not modify the input vector.

#include <cassert>
#include <vector>

// Declare the function prototype for testing.
long long countZeroFilledSubarrays(const std::vector<int>& nums);

int main() {
    // No zeros
    assert(countZeroFilledSubarrays({1, 2, 3}) == 0);

    // Single zero
    assert(countZeroFilledSubarrays({0}) == 1);

    // Two consecutive zeros
    assert(countZeroFilledSubarrays({0, 0}) == 3);

    // Mixed pattern
    assert(countZeroFilledSubarrays({0, 0, 1, 0}) == 4);

    // All zeros
    assert(countZeroFilledSubarrays({0, 0, 0}) == 6);

    // Zeros at both ends
    assert(countZeroFilledSubarrays({0, 1, 0, 1, 0}) == 3);

    // Larger example with multiple streaks
    assert(countZeroFilledSubarrays({0, 1, 0, 0, 2, 0, 0, 0}) == 1 + 3 + 6);

    // Edge case: negative numbers are non-zero
    assert(countZeroFilledSubarrays({-1, -2}) == 0);

    return 0;
}

#include <vector>

// Returns the total number of contiguous subarrays consisting entirely of zeros.
// The input vector is read-only and not modified.
long long countZeroFilledSubarrays(const std::vector<int>& nums) {
    long long total = 0;
    long long currentStreak = 0;

    for (int num : nums) {
        if (num == 0) {
            ++currentStreak;
        } else {
            // A zero-streak has ended; add its subarray count.
            total += currentStreak * (currentStreak + 1) / 2;
            currentStreak = 0;
        }
    }

    // Handle a possible trailing zero-streak.
    total += currentStreak * (currentStreak + 1) / 2;
    return total;
}

// The algorithm scans the vector from left to right, maintaining a running count of consecutive zeros encountered so far (`cnt`). Whenever a non-zero element is found, the current streak of zeros ends, so we add the number of subarrays that can be formed from a streak of length `cnt` to the total answer, then reset `cnt` to zero. The number of contiguous subarrays within a streak of length `k` is given by the triangular number formula `k * (k + 1) / 2`. After the loop ends, we must add the final streak's contribution if the vector ends with zeros. Edge cases include vectors with no zeros (answer 0), a single zero (answer 1), and all zeros (the entire vector is one streak). Time complexity is O(n) with one pass, and space complexity is O(1) auxiliary besides the input vector itself.
