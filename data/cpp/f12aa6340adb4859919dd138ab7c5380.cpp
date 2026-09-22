// Write a C++ function that takes a target amount (in an unspecified currency unit) and three coin denominations (large, medium, small) as unsigned integers, and returns the total number of combinations of non-negative integer counts of these three coins that exactly sum to the target. The denominations are fixed: large = 5000, medium = 2000, small = 1000. The function must handle large target values (up to 2,000,000) efficiently, and return the count as an unsigned integer. If the target is not representable as a sum of these denominations, the function returns 0. The function signature is: `unsigned int countCombinations(unsigned int target);`. The function should not print anything, only compute and return the count.
#include <cassert>

// Forward declaration for testing
unsigned int countCombinations(unsigned int target);

int main() {
    // target = 0: only combination is 0 of each coin
    assert(countCombinations(0) == 1);

    // target = 1000: only 1 small coin
    assert(countCombinations(1000) == 1);

    // target = 2000: either 1 medium or 2 small
    assert(countCombinations(2000) == 2);

    // target = 3000: either 1 medium + 1 small, or 3 small
    assert(countCombinations(3000) == 2);

    // target = 5000: 1 large, or 2 medium + 1 small, or 1 medium + 3 small, or 5 small
    assert(countCombinations(5000) == 4);

    // target = 10000: verified by enumeration: combinations include
    // (2 large), (1 large + 2 med + 1 small), etc. Let's compute via brute force small script
    // Known correct: 10 combinations for 10000
    assert(countCombinations(10000) == 10);

    // target = 200000 (like snippet) — should match the snippet's count (which was 40*100*200 enumeration)
    // The snippet counted combinations; we can compute expected count by a simple loop in the test
    unsigned int expected = 0;
    const unsigned int T = 200000;
    for (unsigned int x = 0; x <= T/5000; ++x)
        for (unsigned int y = 0; y <= T/2000; ++y) {
            unsigned int rem = T - x*5000 - y*2000;
            if (rem % 1000 == 0) ++expected;
        }
    assert(countCombinations(T) == expected);

    // target not representable (e.g., 500) should be 0
    assert(countCombinations(500) == 0);

    // target = 1 (not multiple of 1000) should be 0
    assert(countCombinations(1) == 0);
}
#include <cstdint>

// Counts combinations of 5000, 2000, 1000 coins summing to target.
unsigned int countCombinations(unsigned int target) {
    unsigned int count = 0;
    const unsigned int large = 5000;
    const unsigned int medium = 2000;
    const unsigned int small = 1000;

    for (unsigned int x = 0; x <= target / large; ++x) {
        for (unsigned int y = 0; y <= target / medium; ++y) {
            unsigned int used = x * large + y * medium;
            if (used > target) break; // since y increases, used only grows
            unsigned int remainder = target - used;
            if (remainder % small == 0) {
                // z = remainder / small is automatically non-negative and integer
                ++count;
            }
        }
    }
    return count;
}
// The naïve triple-nested loop over all possible counts of each coin (like the snippet) has time complexity O((target/5000)*(target/2000)*(target/1000)), which becomes huge for target = 200,000 (the loops run about 40 * 100 * 200 = 800,000 iterations, which is fine, but for larger targets it explodes). A better approach is to fix the number of large and medium coins, then check whether the remaining amount is divisible by the small coin (1000) and non-negative. This reduces to two nested loops: `for (x = 0; x <= target/5000; x++)` and `for (y = 0; y <= target/2000; y++)`, and for each pair compute `remainder = target - x*5000 - y*2000`. If `remainder >= 0` and `remainder % 1000 == 0`, then z = remainder/1000 is a valid integer, and we increment the count. This runs in O((target/5000) * (target/2000)) time, which for target = 200,000 is 40 * 100 = 4,000 iterations, and for the maximum 2,000,000 is 400 * 1000 = 400,000 iterations — quite manageable. Edge cases: target smaller than 1000 returns 0; target is exactly a multiple of 1000 but not representable (e.g., 1000 is representable, 2000 is representable, but all multiples of 1000 >= 1000 are representable? Actually yes, because using only 1000-coins works for any multiple of 1000). However, the function still works correctly for any target: if no combination exists, the count remains 0. Space complexity is O(1). The solution uses unsigned arithmetic to avoid overflow; for target up to 2,000,000 and counts up to millions, the count fits in unsigned int (max ~4.29 billion).
