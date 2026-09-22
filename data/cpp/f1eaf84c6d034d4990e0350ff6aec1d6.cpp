// Write a C++ function `int missingNumber(const std::vector<int>& nums)` that, given a vector containing `n` distinct integers chosen from the range `[0, n]`, returns the one integer from that range that is missing. The input vector may be empty (meaning `n = 0`, and the missing number is `0`), may contain numbers in any order, and the integers are guaranteed distinct and within the valid range except for the one missing. The function must be `const`-correct and take the vector by `const` reference to avoid copying. The solution should use only arithmetic, not any sorting or extra data structures.
The mathematical approach is to compute the expected sum of all integers from `0` to `n` (where `n` is the size of the input vector), and subtract the actual sum of the elements in the vector. Since all numbers appear exactly once except the missing one, the difference directly gives the missing value. The expected sum of `0 + 1 + ... + n` is `n*(n+1)/2`, but using a simple loop accumulates without overflow risk for typical `int` sizes (or we can use `long long` for safety). Edge cases: when the vector is empty, `n = 0`, expected sum is `0`, actual sum is `0`, result is `0`. The range always contains exactly `n+1` numbers, and all but one are present. The algorithm runs in O(n) time (one pass to compute actual sum) and O(1) auxiliary space. No special handling for negative numbers is needed because all inputs are non-negative by definition.
#include <vector>

// Return the missing integer in the range [0, n] given n distinct numbers from that range.
int missingNumber(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    long long expectedSum = 0;
    for (int i = 0; i <= n; ++i) {
        expectedSum += i;
    }

    long long actualSum = 0;
    for (int value : nums) {
        actualSum += value;
    }

    return static_cast<int>(expectedSum - actualSum);
}
#include <cassert>
#include <vector>

// The solution function is declared above. Here we test it with various cases.
int main() {
    std::vector<int> v1 = {3, 0, 1};
    assert(missingNumber(v1) == 2);

    std::vector<int> v2 = {0, 1};
    assert(missingNumber(v2) == 2);

    std::vector<int> v3 = {1};
    assert(missingNumber(v3) == 0);

    std::vector<int> v4 = {};
    assert(missingNumber(v4) == 0);

    std::vector<int> v5 = {0, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(missingNumber(v5) == 1);

    std::vector<int> v6 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    assert(missingNumber(v6) == 10);

    std::vector<int> v7 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(missingNumber(v7) == 0);
}
