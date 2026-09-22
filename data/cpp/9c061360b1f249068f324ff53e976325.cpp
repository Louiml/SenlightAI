Write a C++ function that takes two integers `N` and `K`, followed by a list of `N` integers, and returns the sum of all numbers from 1 to K that are **not** present in the list. The function should ignore any input values that are outside the range [1, K] (both inclusive). For example, if K=5 and the list is {3, 1, 9}, the numbers 1 and 3 are present, so the missing numbers are 2,4,5, and their sum is 11. The function should handle large values (up to 10^18) and must not rely on storing all N integers if they are large, but you may store only the relevant ones. The function signature is `long long sumMissing(long long N, long long K, const std::vector<long long>& nums)`. The input vector contains exactly N elements.
// The problem is essentially: compute the total sum of all numbers from 1 to K, then subtract the sum of all distinct numbers in the list that fall within [1, K]. The total sum is given by the formula `K*(K+1)/2`. To avoid double-subtraction when duplicates appear, we use a `std::set` to store only the unique values that are in the valid range. Edge cases: if K is 0, the sum is 0 (since no numbers from 1 to K exist); if the list contains numbers outside [1,K], they are ignored; if the list contains duplicates, they are counted once. Time complexity is O(N log N) due to set insertion, though we can use a `std::unordered_set` for O(N) average, but we'll use `std::set` for determinism. Space complexity is O(min(N, K)) for the set, but in worst case O(N) if all numbers are distinct and in range.
#include <vector>
#include <set>

// Computes the sum of all integers in [1, K] that do not appear in nums.
// Ignores any numbers in nums that are outside [1, K].
long long sumMissing(long long N, long long K, const std::vector<long long>& nums) {
    // Guard against N mismatch (if provided, but the vector size should be N)
    // We'll use the vector size to be safe.
    std::set<long long> present;
    for (long long val : nums) {
        if (val >= 1 && val <= K) {
            present.insert(val);
        }
    }
    
    long long total = K * (K + 1) / 2;
    for (long long val : present) {
        total -= val;
    }
    return total;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Basic case with missing numbers
    assert(sumMissing(3, 5, {3, 1, 9}) == 11);  // 2+4+5

    // All numbers from 1 to K present -> sum missing = 0
    assert(sumMissing(5, 5, {1,2,3,4,5}) == 0);

    // No numbers present -> sum = K*(K+1)/2
    assert(sumMissing(3, 10, {20, -1, 0}) == 55);  // 1..10 sum

    // Duplicates ignored
    assert(sumMissing(4, 4, {2,2,2,4}) == 8); // missing 1+3

    // K = 0 -> sum is 0
    assert(sumMissing(0, 0, {}) == 0);

    // Large numbers, only some in range
    assert(sumMissing(3, 1000000000000LL, {1, 2, 3}) == 499999999999500000LL - 6);

    // Empty vector but N=0
    assert(sumMissing(0, 7, {}) == 28);

    // Numbers exactly at boundaries
    assert(sumMissing(2, 5, {1,5}) == 9); // 2+3+4

    // All duplicates outside range
    assert(sumMissing(2, 3, {7,8}) == 6); // 1+2+3

    // Mixed valid/invalid duplicates
    assert(sumMissing(5, 6, {1,1,2,2,6}) == 12); // 3+4+5
}
