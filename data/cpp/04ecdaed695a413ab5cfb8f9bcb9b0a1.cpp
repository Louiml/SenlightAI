// Write a C++ function `countSkippedNegatives` that takes a vector of integers as input and returns the number of negative numbers that appear while the running sum of all previously considered positive numbers (and any negatives that were absorbed into it) is exactly zero. More precisely, iterate through the array from left to right. Maintain a running sum `S` that starts at 0. Whenever you encounter a positive number, add it to `S`. Whenever you encounter a negative number: if `S` is currently 0, then do NOT add it to `S` and instead count it as one “skipped” negative; if `S` is not 0, then add the negative to `S` (which may reduce `S` but never to a negative value if you follow this rule, but it could become zero again). The function should return the total count of such skipped negatives. For example, for input `[5, -2, -3, 1, -4, -1]`, the running sum is: start S=0, see 5 → S=5, see -2 → S=3, see -3 → S=0, see 1 → S=1, see -4 → S=-3 (but wait this would make S negative, but the original problem doesn't guarantee that can't happen—but we follow the rule literally, so we subtract), then see -1 → S=-4? Actually the rule says if S is zero we skip and count, otherwise add. So we must follow exactly: for every negative, check if S == 0 then count and do not change S; else add to S. Apply this to the given example: [5,-2,-3,1,-4,-1] → S=0, 5→S=5, -2→S=3, -3→S=0, 1→S=1, -4 (S not zero) → S=-3, -1 (S not zero) → S=-4. Count of skipped = 0 because S was never exactly zero when a negative appeared. But for input `[-1, 2, -3]` → start S=0, see -1 (S=0) → skip and count=1, S remains 0; see 2 → S=2; see -3 (S≠0) → S=-1. Count=1. Write a function that implements this logic.

#include <cassert>
#include <vector>

// Declare the function (should be defined in the same translation unit)
int countSkippedNegatives(const std::vector<long long>& values);

int main() {
    // Empty vector
    assert(countSkippedNegatives({}) == 0);
    
    // All negatives: each is skipped because sum stays 0
    assert(countSkippedNegatives({-1, -2, -3}) == 3);
    
    // Positive then negative that cancels: negative is added, not skipped
    assert(countSkippedNegatives({3, -3}) == 0);
    
    // Negative then positive: negative is skipped, positive makes sum non-zero
    assert(countSkippedNegatives({-5, 7}) == 1);
    
    // Mixed case from description: [5, -2, -3, 1, -4, -1] → sum never zero at a negative, count=0
    assert(countSkippedNegatives({5, -2, -3, 1, -4, -1}) == 0);
    
    // Case with zeroes: zero shouldn't affect anything
    assert(countSkippedNegatives({0, -1, 0, 2, -2}) == 1); // -1 skipped, then 2→sum=2, -2→sum=0 (added), count=1
    
    // Multiple skips and absorbs
    assert(countSkippedNegatives({-1, 2, -3, -4}) == 1); // -1 skipped, 2→sum=2, -3→sum=-1, -4→sum=-5
    assert(countSkippedNegatives({1, -1, -2}) == 1); // 1→sum=1, -1→sum=0, -2 skipped (sum=0), count=1
    
    // Large values to test long long
    assert(countSkippedNegatives({1000000000LL, -1000000000LL, -1}) == 1); // sum becomes 0, then -1 skipped
    
    return 0;
}

#include <vector>
#include <cstddef> // for std::size_t

// Counts negative numbers that are skipped because the running sum is zero.
// The running sum is updated only when a positive number is seen or when a
// negative number is seen while the sum is non-zero. Skips count when a negative
// is seen while the sum is exactly zero.
int countSkippedNegatives(const std::vector<long long>& values) {
    long long runningSum = 0;
    int skippedCount = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        long long x = values[i];
        if (x > 0) {
            runningSum += x;
        } else if (x < 0) {
            if (runningSum == 0) {
                ++skippedCount;
            } else {
                runningSum += x;
            }
        }
        // x == 0 has no effect.
    }
    return skippedCount;
}

// The algorithm is a straightforward linear scan. We maintain an integer `runningSum` initialized to 0, and a counter `skippedCount` initialized to 0. For each element `x` in the vector:
// - If `x > 0`, add it to `runningSum`.
// - If `x < 0`:
//   - If `runningSum == 0`, increment `skippedCount` (do not change runningSum).
//   - Else, add `x` to `runningSum`.
// - If `x == 0`, it has no effect (since the condition checks `a[i] < 0`), so we simply ignore it.
// We never reset `runningSum`; we just keep modifying it. The key edge cases include: an empty vector (returns 0), all negatives (every negative is skipped because the sum starts at 0 and never becomes non-zero if only negatives appear—since we never add negatives when sum is zero, so sum stays 0, thus count equals the number of negatives), a positive followed by a negative that exactly cancels it (e.g., [3, -3] → sum becomes 0 after the negative, but that negative is added because sum was 3, not zero; count stays 0), and a negative followed by a positive (the negative is skipped, sum remains 0, then the positive adds to sum). Time complexity is O(n) where n is the number of elements, and space complexity is O(1) extra space (we only use a few variables). The function should be `const` correct by taking a `const std::vector<long long>&` parameter and returning an `int` (or `long long` if the count could be large, but since it's at most the number of negatives, an `int` suffices for typical constraints).
