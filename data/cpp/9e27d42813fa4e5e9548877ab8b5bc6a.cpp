/*
Write a C++ function that, given an integer `n` (with `n >= 2`) and a vector of `n-1` integers representing all but one of the numbers from 1 to `n` (in any order, with no duplicates and no numbers outside this range), returns the missing number. The function must be named `findMissingNumber` and take `int n` and `const std::vector<int>& nums` as parameters, returning an `int`. The solution should not use sorting, must handle large `n` (up to `10^6`) efficiently without overflow, and must work for any permutation of the input values.
*/
#include <vector>
#include <cstdint>

// Returns the missing number from the range 1..n given n-1 numbers in nums.
int findMissingNumber(int n, const std::vector<int>& nums) {
    // Use long long to avoid overflow when n is large (up to 1e6).
    long long total = static_cast<long long>(n) * (n + 1) / 2;
    long long sum = 0;
    for (int value : nums) {
        sum += value;
    }
    return static_cast<int>(total - sum);
}
#include <vector>
#include <cassert>

int findMissingNumber(int n, const std::vector<int>& nums);

int main() {
    // Basic cases.
    assert(findMissingNumber(5, {1, 2, 3, 5}) == 4);
    assert(findMissingNumber(2, {2}) == 1);
    assert(findMissingNumber(2, {1}) == 2);
    assert(findMissingNumber(6, {1, 2, 3, 4, 5}) == 6);
    assert(findMissingNumber(6, {2, 3, 4, 5, 6}) == 1);

    // Random-order permutation with missing in middle.
    assert(findMissingNumber(7, {7, 1, 3, 2, 5, 4}) == 6);

    // Large n to verify no overflow (n=1,000,000, missing = 999,999).
    int largeN = 1000000;
    std::vector<int> largeNums;
    for (int i = 1; i <= largeN; ++i) {
        if (i != 999999) largeNums.push_back(i);
    }
    assert(findMissingNumber(largeN, largeNums) == 999999);

    // n=3, missing 2.
    assert(findMissingNumber(3, {1, 3}) == 2);
}
// The mathematical approach is to compute the expected total sum of all integers from 1 to `n` using the formula `n * (n + 1) / 2` (note: to avoid overflow for large `n`, we should use `long long` for intermediate calculations). Then compute the actual sum of the given vector. The missing number is the difference between the expected total and the actual sum. This works because exactly one number is missing and all others appear once, so the difference directly identifies it. Important edge cases include: `n=2` (vector has one element, either 1 or 2), the missing number being the largest (`n`) or smallest (1), and very large `n` where `n*(n+1)` could overflow a 32-bit integer — hence casting to `long long` before multiplication. Time complexity is O(n) for summing the vector, and space complexity is O(1) beyond the input vector.
