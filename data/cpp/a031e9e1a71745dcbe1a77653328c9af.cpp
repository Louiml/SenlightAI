Write a C++ function that, given a vector of integers containing exactly all distinct integers from `0` to `n` except for one missing value (where `n` is the size of the input vector), returns the missing integer. The input vector is guaranteed to contain `n` elements, each in the range `[0, n]`, with no duplicates, and exactly one integer from that full range is absent. The function should handle any non-negative `n`, including `n = 0` (empty vector, where the missing number is `0`).

The key insight is that the sum of the integers from `0` to `n` is a known arithmetic series formula: `n * (n + 1) / 2`. Since the input vector has size `n`, the full set of numbers it should have contained is `{0, 1, ..., n}` (because the missing number is one of these `n+1` numbers). Compute the total sum of this complete range using the formula, then compute the actual sum of the input vector by iterating over it. The difference between the total sum and the actual sum is exactly the missing number. Edge case: when the vector is empty (`n = 0`), the total sum is `0` and the actual sum is `0`, so the function returns `0`, which is correct because the only possible missing number is `0`. The algorithm runs in `O(n)` time because it makes a single pass over the vector to accumulate its sum, and uses `O(1)` auxiliary space beyond the input itself. No special handling for negative numbers or duplicates is needed because the input contract guarantees they do not appear.

#include <vector>

// Return the missing integer from the range [0, n] when given a vector
// of size n containing all integers in that range except one.
int findMissingNumber(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    int totalSum = n * (n + 1) / 2;
    int actualSum = 0;
    for (int value : nums) {
        actualSum += value;
    }
    return totalSum - actualSum;
}

#include <cassert>
#include <vector>

int main() {
    // Missing number is 0 in a vector containing {1, 2, 3} for n = 3.
    assert(findMissingNumber({1, 2, 3}) == 0);
    // Missing number is 4 in a vector containing {0, 1, 2, 3} for n = 4.
    assert(findMissingNumber({0, 1, 2, 3}) == 4);
    // Missing number is 2 in a vector containing {0, 1, 3} for n = 3.
    assert(findMissingNumber({0, 1, 3}) == 2);
    // Empty vector: n = 0, missing number is 0.
    assert(findMissingNumber({}) == 0);
    // Missing number is 1 in a vector containing {0} for n = 1.
    assert(findMissingNumber({0}) == 1);
    // Larger example: missing 7 from {0..9} except 7.
    assert(findMissingNumber({0, 1, 2, 3, 4, 5, 6, 8, 9}) == 7);
    // Missing number is 0 in a vector containing {1} for n = 1.
    assert(findMissingNumber({1}) == 0);
    // Missing number is 5 in a vector containing {0,1,2,3,4,6} for n = 6.
    assert(findMissingNumber({0, 1, 2, 3, 4, 6}) == 5);
    // Missing number is 9 in a vector containing {0..8} for n = 9.
    assert(findMissingNumber({0, 1, 2, 3, 4, 5, 6, 7, 8}) == 9);
    return 0;
}
