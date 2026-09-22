Write a C++ function named `missingNumber` that takes a positive integer `n` and a vector of `n-1` integers (each in the range 1 to `n`, all distinct, with exactly one number from 1 to `n` missing) and returns the missing integer. For example, if `n = 5` and the vector contains `{2, 3, 1, 5}`, the function should return `4`. The function must handle the smallest possible case `n = 1` (where the vector is empty and the missing number is `1`), and should assume the input is always valid (contains `n-1` distinct integers in range). The function should not modify the input vector and should use only constant extra space.
The problem is a classic "find the missing number" puzzle. Since we know the integers are exactly from 1 to `n` with one missing, the sum of all numbers from 1 to `n` is given by the formula `n*(n+1)/2`. If we compute the actual sum of the provided `n-1` numbers and subtract it from the expected total sum, the difference is the missing number. This works because all numbers are distinct and within range, so no other discrepancy can arise. The edge case `n = 1` is handled naturally: the expected sum is `1*2/2 = 1`, the actual sum of an empty vector is `0`, so the difference is `1`. The algorithm runs in `O(n)` time (one pass to sum the vector) and uses `O(1)` auxiliary space (only a few variables). We must also consider using `long long` for the sums to avoid overflow for large `n` (up to, say, `10^9` in typical constraints), though a `long` is usually sufficient for `n` up to about `10^9` as `n*(n+1)` could exceed 32-bit int.
#include <vector>
#include <cstdint>

// Return the missing integer from a permutation of 1..n with one element omitted.
// The vector contains exactly n-1 distinct integers in [1, n].
long long missingNumber(long long n, const std::vector<int>& numbers) {
    // Compute the full sum of 1..n using the arithmetic series formula.
    long long totalSum = n * (n + 1) / 2;
    
    // Compute the actual sum of the provided numbers.
    long long actualSum = 0;
    for (int value : numbers) {
        actualSum += value;
    }
    
    // The difference is the missing number.
    return totalSum - actualSum;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (from the solution).
long long missingNumber(long long n, const std::vector<int>& numbers);

int main() {
    // Basic case with n=5, missing 4
    assert(missingNumber(5, {2, 3, 1, 5}) == 4);
    // Smallest case: n=1, empty vector, missing 1
    assert(missingNumber(1, {}) == 1);
    // Missing the smallest number 1
    assert(missingNumber(6, {6, 5, 4, 3, 2}) == 1);
    // Missing the largest number n
    assert(missingNumber(4, {1, 2, 3}) == 4);
    // Missing a middle number, n=10, missing 7
    assert(missingNumber(10, {1, 2, 3, 4, 5, 6, 8, 9, 10}) == 7);
    // Large n to check overflow safety (n=1000000, missing 999999)
    std::vector<int> large_numbers;
    for (int i = 1; i <= 1000000; ++i) {
        if (i != 999999) large_numbers.push_back(i);
    }
    assert(missingNumber(1000000, large_numbers) == 999999);
    // n=2, missing 2
    assert(missingNumber(2, {1}) == 2);
    // n=3, missing 3
    assert(missingNumber(3, {1, 2}) == 3);
    // n=2, missing 1
    assert(missingNumber(2, {2}) == 1);
    return 0;
}
