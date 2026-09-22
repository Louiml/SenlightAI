// You are given an integer `n` followed by `n-1` distinct integers from the range `[1, n]`, meaning exactly one number from that range is missing. Write a C++ function `long long missingNumber(long long n, const std::vector<long long>& arr)` that takes `n` and the vector of the `n-1` present numbers (in arbitrary order) and returns the missing number. The function must handle very large `n` (up to 10^10) and must not modify the input vector. Do not assume the input is sorted; the vector may contain any order of the given numbers. The result should fit in a 64-bit signed integer.

#include <cassert>
#include <vector>

// Assuming the solution function is declared above.
int main() {
    // Basic case
    assert(missingNumber(5, {1, 2, 4, 5}) == 3);
    // Missing first number
    assert(missingNumber(4, {2, 3, 4}) == 1);
    // Missing last number
    assert(missingNumber(3, {1, 2}) == 3);
    // Single element case (n=1, empty vector)
    assert(missingNumber(1, {}) == 1);
    // Large n (10^10) with known missing value
    long long bigN = 10000000000LL;
    std::vector<long long> bigVec;
    // Omit the number 7 from 1..bigN by generating all except 7
    for (long long i = 1; i <= bigN; ++i) {
        if (i != 7) bigVec.push_back(i);
    }
    assert(missingNumber(bigN, bigVec) == 7);
    // Unsorted input
    assert(missingNumber(6, {6, 3, 1, 4, 2}) == 5);
    // n=2 with missing 1
    assert(missingNumber(2, {2}) == 1);
    return 0;
}

#include <vector>
#include <cstdint>

// Returns the missing number from 1..n given n-1 distinct numbers in arr.
long long missingNumber(long long n, const std::vector<long long>& arr) {
    long long total = n * (n + 1) / 2; // Sum of 1..n, safe for n up to 1e10
    long long sum = 0;
    for (long long value : arr) {
        sum += value; // Accumulate given numbers
    }
    return total - sum; // Difference is the missing number
}

// The core idea is to use the mathematical formula for the sum of the first `n` natural numbers: `total = n*(n+1)/2`. Since the input contains all numbers from `1` to `n` except one, the missing number is simply `total - sum_of_given_numbers`. This works even if the numbers are not sorted or if duplicates are not present (the problem guarantees distinctness). Edge cases: `n=1` gives an empty vector and the missing number is `1`; `n` can be large (up to 10^10) so use `long long` (64-bit) for all arithmetic to avoid overflow; the sum of given numbers could also be large, so accumulate in a `long long`. The time complexity is O(n-1) for summing the vector, and O(1) auxiliary space (excluding the input vector storage). The approach is both simple and optimal for this problem.
